//
// Created by han on 2/19/21.
//

#include <libtot/libtot.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <chrono>
#include <iostream>
#include <thread>

namespace libtot
{
    User::User(const uint8_t *admin_pubkey_in, const std::string &user_ip, short user_port,
               const std::string &storage_ip, int storage_port)
    {
        set_server_port(user_port);
        set_server_ip(user_ip);

        set_storage_info(storage_ip, storage_port);

        pthread_create(get_thread(USER_THREAD_SERVER), nullptr,
                       server_wrapper, this);

        if (admin_pubkey_in != nullptr)
        {
            memcpy(admin_pubkey, admin_pubkey_in, sizeof(admin_pubkey));
            resolve_ip_kms(admin_pubkey, AsymmetricEncryptionKeySet::FULL_PK_SIZE,
                           admin_ip, admin_port);
            if (admin_port == 0)
            {
                admin_port = default_admin_port;
            }
        }
        // Register user ip and port to KMS
        register_ip_kms();

        LOGV("Client %d is User\n", get_id());
    }

    User::~User() {}

    int User::server_callback_handler(int connection)
    {
        MessageType incoming = network_read_message_type(connection);

        if (incoming == MessageType_DATA_STORE_SIEVE_CRED_REQUEST)
        {
            return data_store_handler(connection);
        }
        else if (incoming == MessageType_DATA_ACCESS_FETCH)
        {
            return data_access_handler(connection);
        }

        return 0;
    }

    int User::acquire_pre_auth_token()
    {
        return user_acquire_pre_auth_token_impl(admin_ip.c_str(), admin_port, admin_pubkey,
                                                get_keyset(), pre_auth_token);
    }

    int User::claim_device(bool exclusive, std::string device_pubkey_b64)
    {
        std::string device_ip = default_device_ip;
        int device_port = default_device_port;

        if (!device_pubkey_b64.empty())
        {
            std::string device_pubkey = base64_decode(device_pubkey_b64);
            resolve_ip_kms(reinterpret_cast<const uint8_t *>(device_pubkey.c_str()),
                           device_pubkey.size(), device_ip, device_port);
            if (device_port == 0)
            {
                device_port = default_device_port;
            }
        }

        return user_claim_device_impl(get_keyset(), pre_auth_token, device_ip.c_str(), device_port,
                                      admin_pubkey, exclusive, claimed_device, sizeof(claimed_device));
    }

    int User::data_store_handler(int connection)
    {
        uint8_t request_buf[READ_BUFFER_SIZE];
        network_read(connection, request_buf, sizeof(request_buf));
        auto request_msg = GetDataStoreSieveCredRequest(request_buf);

        CiphertextDataStoreSieveCredRequest request_payload;
        get_keyset().box_open_easy(reinterpret_cast<uint8_t *>(&request_payload), sizeof(request_payload),
                                   request_msg->ciphertext()->Data(), request_msg->ciphertext()->size(),
                                   request_msg->session_nonce()->Data(), request_msg->device_pubkey()->Data());

        if (request_payload.type != CipherType::data_store_sieve_cred_request)
        {
            LOGW("Unexpected cred request type");
            return -1;
        }

        SharedSecretKey session_key(request_payload.session_key, sizeof(request_payload.session_key));

        if (memcmp(claimed_device, request_msg->device_pubkey()->Data(), sizeof(claimed_device)) != 0)
        {
            LOGW("Didn't claim this device before. Abort.");
            return -1;
        }

        SieveKey sieve_key;

        CiphertextDataStoreSieveCredResponse response_payload;
        response_payload.type = CipherType::data_store_sieve_cred_response;
        sieve_key.serialize_key_into(response_payload.sieve_key, sizeof(response_payload.sieve_key));
        sieve_key.serialize_nonce_into(response_payload.sieve_nonce, sizeof(response_payload.sieve_nonce));

        size_t response_cipher_len = SharedSecretKey::get_cipher_len(sizeof(response_payload));
        auto response_cipher = new uint8_t[response_cipher_len]{};
        session_key.encrypt(response_cipher, response_cipher_len,
                            reinterpret_cast<uint8_t *>(&response_payload),
                            sizeof(response_payload));

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto response_session_header_obj = builder.CreateVector(session_key.get_header(),
                                                                SharedSecretKey::HEADER_SIZE);
        auto response_cipher_obj = builder.CreateVector(response_cipher, response_cipher_len);
        auto response_msg = CreateDataStoreSieveCredResponse(builder, response_session_header_obj,
                                                             response_cipher_obj);
        builder.Finish(response_msg);

        network_send_message_type(connection, MessageType_DATA_STORE_SIEVE_CRED_RESPONSE);
        network_send(connection, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(connection) != MessageType_DATA_STORE_UPLOAD_NOTIFICATION)
        {
            LOGW("Unexpected data upload notification message type");
            return -1;
        }

        uint8_t notification_buf[READ_BUFFER_SIZE];
        network_read(connection, notification_buf, sizeof(notification_buf));
        auto notification_msg = DataStoreUpload::GetDataStoreUploadNotification(notification_buf);

        session_key.load_header_decryption(notification_msg->session_header_refresh()->Data(),
                                           notification_msg->session_header_refresh()->size());
        CiphertextDataStoreUploadNotification notification_payload;
        session_key.decrypt(reinterpret_cast<uint8_t *>(&notification_payload), sizeof(notification_payload),
                            notification_msg->ciphertext()->Data(), notification_msg->ciphertext()->size());

        if (notification_payload.type != CipherType::data_store_upload_notification)
        {
            LOGW("Unexpected data store notification type");
            return -1;
        }

        UUID sieve_data_UUID = UUID(notification_payload.sieve_data_block_uuid,
                                    sizeof(notification_payload.sieve_data_block_uuid));
        UUID enc_meta_UUID = UUID(notification_payload.encrypted_metadata_uuid,
                                  sizeof(notification_payload.encrypted_metadata_uuid));

        // Store session key and ID for later use...
        sieve_key_table[sieve_data_UUID] = sieve_key;
        enc_meta_block_table[sieve_data_UUID] = enc_meta_UUID;

        return 0;
    }

    int User::data_access_handler(int conn)
    {
        // Construct request input
        uint8_t fetch_buf[READ_BUFFER_SIZE]{};
        network_read(conn, fetch_buf, sizeof(fetch_buf));
        auto fetch_msg = GetDataAccessFetch(fetch_buf);

        CiphertextDataAccessFetch fetch_payload;
        get_keyset().box_open_easy(reinterpret_cast<uint8_t *>(&fetch_payload), sizeof(fetch_payload),
                                   fetch_msg->ciphertext()->data(), fetch_msg->ciphertext()->size(),
                                   fetch_msg->message_nonce()->data(), fetch_msg->accessor_pubkey()->data());

        if (fetch_payload.type != CipherType::data_access_fetch)
        {
            LOGW("Wrong fetch request type");
            return -1;
        }

        UUID sieve_uuid(fetch_payload.sieve_data_block_uuid,
                        sizeof(fetch_payload.sieve_data_block_uuid));

        if (!delegate_access(sieve_uuid,
                             fetch_msg->accessor_pubkey()->data(),
                             fetch_msg->accessor_pubkey()->size()))
        {
            LOGI("Access denied");
            return -1;
        }

        if (sieve_key_table.find(sieve_uuid) == sieve_key_table.end())
        {
            LOGW("Sieve uuid not found: %s", sieve_uuid.get_uuid().c_str());
            return -1;
        }

        CiphertextDataAccessResponse response;
        response.type = CipherType::data_access_response;
        sieve_key_table[sieve_uuid].serialize_key_into(response.sieve_key, sizeof(response.sieve_key));
        memcpy(response.random_challenge_response,
               fetch_payload.random_challenge,
               sizeof(fetch_payload.random_challenge));

        uint8_t msg_nonce[AsymmetricEncryptionKeySet::NONCE_SIZE]{};

        size_t cipher_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(sizeof(response));
        auto cipher = new uint8_t[cipher_len]{};
        get_keyset().box_easy(cipher, cipher_len, reinterpret_cast<const uint8_t *>(&response),
                              sizeof(response), msg_nonce, fetch_msg->accessor_pubkey()->data());

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto msg_nonce_obj = builder.CreateVector(msg_nonce, sizeof(msg_nonce));
        auto ciphertext_obj = builder.CreateVector(cipher, cipher_len);
        auto response_msg = CreateDataAccessResponse(builder, msg_nonce_obj, ciphertext_obj);
        builder.Finish(response_msg);

        network_send_message_type(conn, MessageType_DATA_ACCESS_RESPONSE);
        network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        return 0;
    }

    bool User::delegate_access(const UUID &sieve_uuid, const uint8_t *request_pubkey, size_t request_pubkey_len)
    {
        // TODO
        return true;
    }

    int User::re_encrypt(const UUID &sieve_data_block_uuid, const uint8_t *storage_pk)
    {
        uint8_t user_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
        get_keyset().get_full_pk(user_pubkey, sizeof(user_pubkey));

        if (sieve_key_table.find(sieve_data_block_uuid) == sieve_key_table.end() ||
            enc_meta_block_table.find(sieve_data_block_uuid) == enc_meta_block_table.end())
        {
            LOGW("Sieve data UUID not found");
            return -1;
        }

        auto re_encrypt_target = enc_meta_block_table[sieve_data_block_uuid];

        SieveKey sieve_key_new;
        RekeyToken token = sieve_key_table[sieve_data_block_uuid].gen_rekey_token(sieve_key_new);

        const uint8_t *dynamic_pk = storage_pk;
        if (dynamic_pk == nullptr)
        {
            // Fetch storage's public key through out-of-band trusted KMS
            int conn = network_connect(get_storage_ip().c_str(), get_storage_port());
            network_send_message_type(conn, MessageType_UTIL_FETCH_STORE_PUBKEY);
            uint8_t store_pk_buf[READ_BUFFER_SIZE]{};
            network_read(conn, store_pk_buf, sizeof(store_pk_buf));
            auto msg = GetUtilFetchStorePubkey(store_pk_buf);
            auto buf = new uint8_t[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
            memcpy(buf, msg->pubkey()->data(), AsymmetricEncryptionKeySet::FULL_PK_SIZE);
            dynamic_pk = buf;
        }

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        size_t cipher_len = 0;
        uint8_t *cipher = nullptr;
        int conn = network_connect(get_storage_ip().c_str(), get_storage_port());

        /**
         * Negotiate pre-request to prevent replay attack
         */
        // Send pre request
        CiphertextDataReencryptionPreRequest pre_req_payload;
        pre_req_payload.type = CipherType::data_reencryption_pre_request;
        memcpy(pre_req_payload.sieve_data_block_uuid,
               sieve_data_block_uuid.get_uuid().c_str(),
               sizeof(pre_req_payload.sieve_data_block_uuid));
        memcpy(pre_req_payload.encrypted_metadata_uuid,
               enc_meta_block_table[sieve_data_block_uuid].get_uuid().c_str(),
               sizeof(pre_req_payload.encrypted_metadata_uuid));
        random_buf(pre_req_payload.user_nonce, sizeof(pre_req_payload.user_nonce));

        cipher_len = AsymmetricEncryptionKeySet::get_box_seal_cipher_len(sizeof(pre_req_payload));
        delete[] cipher;
        cipher = new uint8_t[cipher_len]{0};

        get_keyset().box_seal(cipher, cipher_len,
                              reinterpret_cast<const uint8_t *>(&pre_req_payload),
                              sizeof(pre_req_payload), dynamic_pk);

        builder.Clear();
        auto pre_req_cipher_obj = builder.CreateVector(cipher, cipher_len);
        auto pre_req_msg = CreateDataReencryptionPreRequest(builder, pre_req_cipher_obj);
        builder.Finish(pre_req_msg);

        network_send_message_type(conn, MessageType_DATA_REENCRYPTION_PRE_REQUEST);
        network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        // Process pre response
        if (network_read_message_type(conn) != MessageType_DATA_REENCRYPTION_PRE_RESPONSE)
        {
            LOGW("Unexpected message for Pre-response!");
            return -1;
        }
        uint8_t pre_res_buf[READ_BUFFER_SIZE]{0};
        network_read(conn, pre_res_buf, sizeof(pre_res_buf));
        auto pre_res_msg = GetDataReencryptionPreResponse(pre_res_buf);
        CiphertextDataReencryptionPreResponse pre_res_payload;
        get_keyset().box_seal_open(reinterpret_cast<uint8_t *>(&pre_res_payload),
                                   sizeof(pre_res_payload),
                                   pre_res_msg->ciphertext()->data(),
                                   pre_res_msg->ciphertext()->size());

        if (pre_res_payload.type != CipherType::data_reencryption_pre_response)
        {
            LOGW("Wrong message type for pre response");
            return -1;
        }

        if (memcmp(pre_res_payload.user_nonce,
                   pre_req_payload.user_nonce,
                   sizeof(pre_res_payload.user_nonce)) != 0)
        {
            LOGW("Incorrect user nonce responded");
            return -1;
        }

        /**
         * Construct the main request
         */
        CiphertextDataReencryptionRequest request_payload;
        request_payload.type = CipherType::data_reencryption_request;
        memcpy(&(request_payload.rekey_token),
               reinterpret_cast<const uint8_t *>(&token),
               sizeof(token));
        random_buf(request_payload.noti_token, G_CHALLENGE_SIZE);
        memcpy(request_payload.user_nonce,
               pre_req_payload.user_nonce,
               sizeof(request_payload.user_nonce));
        memcpy(request_payload.storage_nonce,
               pre_res_payload.storage_nonce,
               sizeof(request_payload.storage_nonce));

        LOGV("Rekey token size: %d", sizeof(request_payload.rekey_token));

        cipher_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(sizeof(request_payload));
        delete[] cipher;
        cipher = new uint8_t[cipher_len]{0};
        uint8_t nonce[AsymmetricEncryptionKeySet::NONCE_SIZE]{0};

        hexprint(dynamic_pk, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
        get_keyset().box_easy(cipher, cipher_len, reinterpret_cast<const uint8_t *>(&request_payload),
                              sizeof(request_payload), nonce, dynamic_pk);

        builder.Clear();
        auto sieve_uuid_obj = builder.CreateString(re_encrypt_target.get_uuid());
        auto owner_pk_obj = builder.CreateVector(user_pubkey, sizeof(user_pubkey));
        auto msg_nonce_obj = builder.CreateVector(nonce, sizeof(nonce));
        auto cipher_obj = builder.CreateVector(cipher, cipher_len);
        auto request_msg = CreateDataReencryptionRequest(builder, sieve_uuid_obj, owner_pk_obj, msg_nonce_obj, cipher_obj);
        builder.Finish(request_msg);

        network_send_message_type(conn, MessageType_DATA_REENCRYPTION_REQUEST);
        network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(conn) != MessageType_DATA_REENCRYPTION_RESPONSE)
        {
            LOGW("Wrong reencryption response message type");
            return -1;
        }
        uint8_t res_buf[READ_BUFFER_SIZE]{0};
        network_read(conn, res_buf, sizeof(res_buf));
        auto res_msg = GetDataReencryptionResponse(res_buf);

        if (memcmp(res_msg->notification_token()->data(),
                   request_payload.noti_token,
                   sizeof(request_payload.noti_token)) != 0)
        {
            LOGW("Unmatched notification token!");
            return -1;
        }

        delete[] cipher;
        delete[] dynamic_pk;

        sieve_key_table[sieve_data_block_uuid].apply_rekey_token_key(token);

        return 0;
    }

    void User::wait_all()
    {
        void *status;

        LOGI("User %d is waiting for all threads...", get_id());

        for (unsigned long thread : threads)
        {
            pthread_join(thread, &status);
        }
    }

    int User::sign_access_cert(const uint8_t *msg, size_t msg_len, uint8_t **cert_ptr, size_t *cert_len_ptr)
    {
        if (*cert_ptr != nullptr)
        {
            delete[](*cert_ptr);
        }

        *cert_len_ptr = AsymmetricEncryptionKeySet::SIGNATURE_SIZE;
        (*cert_ptr) = new uint8_t[*cert_len_ptr]{};
        std::cout << "About to sign" << std::endl;
        get_keyset().sign_detached(*cert_ptr, msg, msg_len);
        std::cout << "Finish sign" << std::endl;

        return 0;
    }

}