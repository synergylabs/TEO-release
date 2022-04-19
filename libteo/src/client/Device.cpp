//
// Created by han on 2/11/21.
//

#include <teo/teo.hpp>
#include <teo/client/Device.hpp>
#include <teo/teo_semaphore.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <queue>
#include <condition_variable>
#include <map>
#include <stdlib.h>

namespace teo
{
    Device::Device()
    {
        set_server_port(default_device_port);

        // We have to do this first since creating BLE Beacon requires sudo password
#if defined(TEO_BLUETOOTH_BEACON)
        pthread_create(get_thread(DEVICE_THREAD_BEACON), nullptr,
                       beacon_wrapper, this);
#endif

        pthread_create(get_thread(DEVICE_THREAD_SERVER), nullptr,
                       server_wrapper, this);

        pthread_create(get_thread(DEVICE_THREAD_INFO_PRITNER), nullptr,
                       print_info_wrapper, this);

        LOGV("Client %d is Device", get_id());
    }

    Device::Device(SharedSecretKey &setup_token_in, const std::string &storage_ip,
                   int storage_port) : Device()
    {
        setup_token = setup_token_in;

        set_storage_info(storage_ip, storage_port);
        register_ip_kms();
    }

    Device::~Device()
    {
        flush_owner_key();
    }

    void *Device::print_info_wrapper(void *obj)
    {
        reinterpret_cast<Device *>(obj)->print_info();
        return nullptr;
    }

    void *Device::beacon_wrapper(void *obj)
    {
        reinterpret_cast<Device *>(obj)->launch_beacon();
        return nullptr;
    }

    int Device::launch_beacon()
    {
        const std::string BEACON_PREFIX = "sudo hcitool -i hci0 cmd 0x08 0x0008 1f 02 01 06 03 03 aa fe 17 16 aa fe 10 00";

        std::string device_pubkey_b64 = base64_encode(get_keyset().get_full_pk());

        while (true)
        {
            std::stringstream ss;
            ss << BEACON_PREFIX;

            std::unique_lock<std::mutex> data_lock(g_data_mutex, std::defer_lock);
            data_lock.lock();

            // Update/refresh beacon nonces
            std::string msg;
            msg.reserve(G_BEACON_MSG_LEN);
            msg = "TOTB";
            for (int i = 0; i < G_BEACON_ID_LEN; i++)
            {
                msg.push_back(device_pubkey_b64[i]);
            }
            msg += random_string(G_BEACON_NONCE_LEN);
            beacon_msgs.push_back(msg);

            while (beacon_msgs.size() > G_BEACON_MSG_HISTORY)
            {
                beacon_msgs.pop_front();
            }

            for (auto c : beacon_msgs.back())
            {
                ss << " " << std::hex << (int)c;
            }

            // Proximity manager, remove outdated owners
            if (G_BEACON_PROXIMITY_ENABLE)
            {
                for (auto const &it : owner_hb_count)
                {
                    if (it.second >= G_BEACON_PROXIMITY_TIMEOUT_LIM)
                    {
                        LOGD(("Removing owner due to proximity inactivity " + it.first).c_str());
                        release_device_owner(it.first);
                        owner_hb_count.erase(it.first);
                    }
                    else
                    {
                        owner_hb_count[it.first]++;
                    }
                }
            }

            data_lock.unlock();

            system(ss.str().c_str());

            std::this_thread::sleep_for(std::chrono::seconds(G_BEACON_REF_INT));
        }

        return 0;
    }

    int Device::process_heartbeat_handler(int connection)
    {
        uint8_t heartbeat_buf[READ_BUFFER_SIZE]{};
        network_read(connection, heartbeat_buf, READ_BUFFER_SIZE);
        auto heartbeat_msg = GetUtilHeartbeat(heartbeat_buf);

        std::string n_p = std::string(reinterpret_cast<const char *>(heartbeat_msg->proximity_nonce()->data()),
                                      heartbeat_msg->proximity_nonce()->size());
        auto owner_pk = heartbeat_msg->user_pubkey()->data();
        std::string owner_pk_b64 = base64_encode(owner_pk, heartbeat_msg->user_pubkey()->size());

        std::unique_lock<std::mutex> data_lock(g_data_mutex, std::defer_lock);
        data_lock.lock();

        for (int i = 0; i < owner_keys.size(); i++)
        {
            auto ok = owner_keys[i];
            std::string ok_b64 = base64_encode(ok, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

            if (ok_b64 == owner_pk_b64)
            {
                int i = 0;
                for (auto rit = beacon_msgs.rbegin(); rit != beacon_msgs.rend(); rit++)
                {
                    if (*rit == n_p)
                    {
                        owner_hb_count[ok_b64] = std::min(owner_hb_count[ok_b64], i);
                    }

                    i++;
                }
                break;
            }
        }

        data_lock.unlock();

        return 0;
    }

    int Device::print_info()
    {
        while (true)
        {
            LOGV("*****************************************************************\n");

            LOGV("Current device information:\n");
            LOGV("admin: %s\n", get_admin_key_b64().c_str());
            std::string owner_keys_str = "";
            for (auto owner : owner_keys)
            {
                owner_keys_str += "\t";
                owner_keys_str += base64_encode(owner, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
                owner_keys_str += "\n";
            }
            LOGV("owner keys: \n{%s}\n", owner_keys_str.c_str());

            LOGV("*****************************************************************\n");

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    int Device::set_owner(uint8_t *pk, bool group_mode)
    {
        if (!group_mode)
        {
            // Excuslively set owner
            flush_owner_key();
        }
        auto pk_in = new uint8_t[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
        memcpy(pk_in, pk, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

        // Check if owner is already here
        for (auto owner : owner_keys)
        {
            if (memcmp(owner, pk_in, AsymmetricEncryptionKeySet::FULL_PK_SIZE) == 0)
            {
                LOGV("Owner's key already exists!");
                return 0;
            }
        }

        owner_keys.push_back(pk_in);

        return 0;
    }

    void Device::flush_owner_key()
    {
        // Flush existing owners
        for (auto &o : owner_keys)
        {
            if (o != nullptr)
            {
                delete[] o;
            }
        }
        owner_keys.clear();
    }

    bool Device::is_group()
    {
        return owner_keys.size() > 1;
    }

    int Device::server_callback_handler(int connection)
    {
        MessageType incoming = network_read_message_type(connection);

        if (incoming == MessageType_INITIALIZATION_REQUEST)
        {
            return accept_initialization_handler(connection);
        }
        else if (incoming == MessageType_CLAIM_DEVICE_DISCOVERY)
        {
            return acquire_ownership_handler(connection);
        }
        else if (incoming == MessageType_UTIL_RELEASE_DEVICE)
        {
            return release_device_handler(connection);
        }
        else if (incoming == MessageType_UTIL_REMOVE_REAL_TIME_ACCESS)
        {
            return remove_real_time_access_handler(connection);
        }
        else if (incoming == MessageType_UTIL_HEARTBEAT)
        {
            return process_heartbeat_handler(connection);
        }

        return 0;
    }

    int Device::release_device_handler(int connection)
    {
        uint8_t request_buf[READ_BUFFER_SIZE]{};
        network_read(connection, request_buf, READ_BUFFER_SIZE);
        auto request_msg = GetUtilReleaseDevice(request_buf);

        auto owner_pk = request_msg->user_pubkey()->data();
        std::string owner_pk_b64 = base64_encode(owner_pk, request_msg->user_pubkey()->size());

        release_device_owner(owner_pk_b64);

        return 0;
    }

    int Device::release_device_owner(std::string owner_pk_b64)
    {

        for (int i = 0; i < owner_keys.size(); i++)
        {
            auto ok = owner_keys[i];
            std::string ok_b64 = base64_encode(ok, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

            if (ok_b64 == owner_pk_b64)
            {
                owner_keys.erase(owner_keys.begin() + i);
                real_time_perm.erase(real_time_perm.find(owner_pk_b64));
            }
        }

        return 0;
    }

    int Device::remove_real_time_access_handler(int connection)
    {
        uint8_t request_buf[READ_BUFFER_SIZE]{};
        network_read(connection, request_buf, READ_BUFFER_SIZE);
        auto request_msg = GetUtilRemoveRealTimeAccess(request_buf);
        auto owner_pk = request_msg->user_pubkey()->data();
        std::string owner_pk_b64 = base64_encode(owner_pk, request_msg->user_pubkey()->size());

        auto it = real_time_perm.find(owner_pk_b64);
        if (it != real_time_perm.end())
        {
            real_time_perm[owner_pk_b64] = false;
        }

        return 0;
    }

    int Device::accept_initialization_handler(int connection)
    {
        uint8_t request_buf[READ_BUFFER_SIZE]{};
        network_read(connection, request_buf, READ_BUFFER_SIZE);
        auto request_msg = GetInitializationRequest(request_buf);

        setup_token.load_header_decryption(request_msg->setup_header()->Data(),
                                           request_msg->setup_header()->size());

        CiphertextInitializationRequest request_payload;
        setup_token.decrypt(reinterpret_cast<unsigned char *>(&request_payload), sizeof(request_payload),
                            request_msg->ciphertext()->Data(), request_msg->ciphertext()->size());

        if (request_payload.type != CipherType::initialization_request)
        {
            LOGV("Incorrect type\n");
            return -1;
        }

        CiphertextInitializationDeviceInfo device_info_payload;
        device_info_payload.type = CipherType::initialization_device_info;
        memcpy(device_info_payload.admin_challenge, request_payload.admin_challenge, G_CHALLENGE_SIZE);
        random_buf(device_info_payload.device_challenge, G_CHALLENGE_SIZE);
        get_keyset().get_full_pk(device_info_payload.device_pubkey, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

        size_t device_info_ciphertext_len = get_keyset().get_box_seal_cipher_len(sizeof(device_info_payload));
        auto device_info_ciphertext = new uint8_t[device_info_ciphertext_len];
        get_keyset().box_seal(device_info_ciphertext, device_info_ciphertext_len,
                              reinterpret_cast<const uint8_t *>(&device_info_payload), sizeof(device_info_payload),
                              request_payload.admin_pubkey);

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto device_info_ciphertext_obj = builder.CreateVector(device_info_ciphertext, device_info_ciphertext_len);
        auto device_info_msg = CreateInitializationDeviceInfo(builder, device_info_ciphertext_obj);
        builder.Finish(device_info_msg);

        network_send_message_type(connection, MessageType_INITIALIZATION_DEVICE_INFO);
        send(connection, builder.GetBufferPointer(), builder.GetSize(), SOCKET_SEND_FLAGS);

        if (network_read_message_type(connection) != MessageType_INITIALIZATION_ADMIN_REPLY)
        {
            LOGV("Unexpected admin reply\n");
            return -1;
        }

        uint8_t admin_reply_buf[READ_BUFFER_SIZE];
        network_read(connection, admin_reply_buf, sizeof(admin_reply_buf));
        auto admin_reply_msg = GetInitializationAdminReply(admin_reply_buf);

        CiphertextInitializationAdminReply admin_reply_payload;
        if (get_keyset().box_open_easy(reinterpret_cast<uint8_t *>(&admin_reply_payload), sizeof(admin_reply_payload),
                                       admin_reply_msg->ciphertext()->Data(),
                                       admin_reply_msg->ciphertext()->size(), admin_reply_msg->msg_nonce()->Data(),
                                       request_payload.admin_pubkey) != 0)
        {
            LOGV("Failed to open admin reply\n");
            return -1;
        }

        if (admin_reply_payload.type != CipherType::initialization_admin_reply ||
            memcmp(admin_reply_payload.device_challenge, device_info_payload.device_challenge, G_CHALLENGE_SIZE) != 0)
        {
            LOGV("Failed to check matching type or device challenge\n");
            return -1;
        }

        memcpy(admin_key, request_payload.admin_pubkey, sizeof(admin_key));
        memcpy(valid_device_proof, admin_reply_payload.device_proof, sizeof(valid_device_proof));

        LOGV("Event: Initialization device accept admin\n");

        delete[] device_info_ciphertext;

        return 0;
    }

    int Device::acquire_ownership_handler(int connection)
    {
        // Load self pubkey
        uint8_t device_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE];
        get_keyset().get_full_pk(device_pubkey, sizeof(device_pubkey));

        // Fetch discovery request message
        uint8_t discovery_buf[READ_BUFFER_SIZE];
        network_read(connection, discovery_buf, sizeof(discovery_buf));
        auto discovery_msg = GetClaimDeviceDiscovery(discovery_buf);

        // Generate discovery response back to user
        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto device_pubkey_obj = builder.CreateVector(device_pubkey, sizeof(device_pubkey));
        auto device_proof_obj = builder.CreateVector(valid_device_proof, sizeof(valid_device_proof));
        auto discovery_response_msg = CreateClaimDeviceDiscoveryResponse(builder,
                                                                         device_pubkey_obj,
                                                                         device_proof_obj);
        builder.Finish(discovery_response_msg);
        network_send_message_type(connection, MessageType_CLAIM_DEVICE_DISCOVERY_RESPONSE);
        network_send(connection, builder.GetBufferPointer(), builder.GetSize());

        // Fetch and process user actual claiming request
        if (network_read_message_type(connection) != MessageType_CLAIM_DEVICE_REQUEST)
        {
            LOGW("Failed to get user's follow up request\n");
            return -1;
        }
        uint8_t request_buf[READ_BUFFER_SIZE];
        network_read(connection, request_buf, sizeof(request_buf));
        auto request_msg = GetClaimDeviceRequest(request_buf);

        CiphertextClaimDeviceRequest request_payload;
        get_keyset().box_open_easy(reinterpret_cast<uint8_t *>(&request_payload), sizeof(request_payload),
                                   request_msg->ciphertext()->Data(), request_msg->ciphertext()->size(),
                                   request_msg->nonce()->Data(), discovery_msg->user_pubkey()->Data());

        if (request_payload.type != CipherType::claim_device_request)
        {
            LOGW("Incorrect request payload type\n");
            return -1;
        }

        // Verify user's pre-auth token. Only allow trusted users to claim this device.
        SignaturePreAuthToken user_token_content;
        memcpy(user_token_content.user_pubkey, discovery_msg->user_pubkey()->Data(),
               sizeof(user_token_content.user_pubkey));
        if (AsymmetricEncryptionKeySet::sign_verify_detached(request_payload.token,
                                                             sizeof(request_payload.token),
                                                             reinterpret_cast<uint8_t *>(&user_token_content),
                                                             sizeof(user_token_content),
                                                             admin_key) != 0)
        {
            LOGW("Invalid pre auth token from user\n");
            return -1;
        }

        LOGV("Event: Claim device device accept user");

        // Transfer ownership to new user
        set_owner(user_token_content.user_pubkey,
                  request_msg->group_mode() > 0);

        // Send back notification to end-user
        CiphertextClaimDeviceResponse response_payload;
        response_payload.type = CipherType::clain_device_response;
        memcpy(response_payload.challenge_decrypted, request_payload.user_challenge,
               sizeof(request_payload.user_challenge));
        size_t ciphertext_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(sizeof(response_payload));
        auto ciphertext = new uint8_t[ciphertext_len];
        uint8_t nonce[AsymmetricEncryptionKeySet::NONCE_SIZE];
        get_keyset().box_easy(ciphertext, ciphertext_len,
                              reinterpret_cast<uint8_t *>(&response_payload), sizeof(response_payload),
                              nonce, user_token_content.user_pubkey);

        builder.Clear();
        auto nonce_obj = builder.CreateVector(nonce, sizeof(nonce));
        auto ciphertext_obj = builder.CreateVector(ciphertext, ciphertext_len);
        auto response_msg = CreateClaimDeviceResponse(builder, true, nonce_obj, ciphertext_obj);
        builder.Finish(response_msg);

        network_send_message_type(connection, MessageType_CLAIM_DEVICE_RESPONSE);
        network_send(connection, builder.GetBufferPointer(), builder.GetSize());

        delete[] ciphertext;

        return 0;
    }

    int Device::store_data(const std::string &file_path,
                           UUID *sieve_block_result,
                           int *sieve_enc_timer,
                           int *sym_enc_timer,
                           int *upload_timer,
                           int *sieve_nego_timer,
                           int *upload_notify_timer)
    {
        return store_data_tot_impl(sieve_block_result,
                                   file_path,
                                   nullptr,
                                   0,
                                   sieve_enc_timer,
                                   sym_enc_timer,
                                   upload_timer,
                                   sieve_nego_timer,
                                   upload_notify_timer);
    }

    int Device::store_data(const uint8_t *file_content_ptr,
                           size_t file_content_len,
                           UUID *sieve_block_result,
                           int *sieve_enc_timer,
                           int *sym_enc_timer,
                           int *upload_timer,
                           int *sieve_nego_timer,
                           int *upload_notify_timer)
    {
        return store_data_tot_impl(sieve_block_result,
                                   "",
                                   file_content_ptr,
                                   file_content_len,
                                   sieve_enc_timer,
                                   sym_enc_timer,
                                   upload_timer,
                                   sieve_nego_timer,
                                   upload_notify_timer);
    }

    struct DSComplete
    {
        size_t idx;
        size_t cipher_len;
        uint8_t *cipher_buf;
    };

    int Device::store_data_tot_impl(UUID *sieve_block_result,
                                    const std::string &file_path,
                                    const uint8_t *input_buf,
                                    size_t input_buf_len,
                                    int *sieve_enc_timer,
                                    int *sym_enc_timer,
                                    int *upload_timer,
                                    int *sieve_nego_timer,
                                    int *upload_notify_timer)
    {
        /**
         * Input arguments sanity check
         */
        // Either file_path or input_buf need to be set, not both
        assert(!(file_path.empty()) || (input_buf != nullptr));

        bool stopwatch = ((sieve_enc_timer != nullptr) &&
                          (sym_enc_timer != nullptr) &&
                          (upload_timer != nullptr) &&
                          (sieve_nego_timer != nullptr) &&
                          (upload_notify_timer != nullptr));
        if (stopwatch)
        {
            *sieve_enc_timer = 0;
            *sym_enc_timer = 0;
            *upload_timer = 0;
            *sieve_nego_timer = 0;
            *upload_notify_timer = 0;
        }
        std::chrono::high_resolution_clock::time_point parent_timer_start, parent_timer_stop;

        // Load payload into memory
        size_t content_len = 0;
        const uint8_t *content_buf = nullptr;
        if (file_path.empty())
        {
            // Load directly from buffer
            content_len = input_buf_len;
            content_buf = input_buf;
        }
        else
        {
            // Read from file
            std::ifstream file;
            file.open(file_path, std::ios::in | std::ios::binary | std::ios::ate);

            if (!file.is_open())
            {
                LOGW("Failed to open file %s", file_path.c_str());
                return -1;
            }
            std::streampos file_size = file.tellg();
            file.seekg(0, std::ios::beg);

            content_len = (size_t)file_size;
            uint8_t *buf = new uint8_t[content_len]{};
            if (!file.read((char *)buf, content_len))
            {
                LOGW("Error reading file %s", file_path.c_str());
                return -1;
            }
            content_buf = buf;
        }

        /**
         * Main data store workflow
         * System design:
         *  - One thread encrypt data in chunks, push the completion notification to the queue
         *  - One thread to upload data chunks to the storage provider, store the UUID to another list
         */
        if (content_len >= G_MAX_FILE_SIZE)
        {
            LOGW("Storage file too large! Currently unsupported!!");
            return -1;
        }

        int total_chunks = (content_len + G_FILE_CHUNK_SIZE - 1) / G_FILE_CHUNK_SIZE;

        std::queue<DSComplete *> completion_queue;
        Semaphore comp_q_smph;

        std::map<size_t, UUID *> data_chunk_uuid_m;

        SharedSecretKey data_key;

        auto enc_lambda = [&]()
        {
            std::unique_lock<std::mutex> data_lock(g_data_mutex, std::defer_lock);
            std::chrono::high_resolution_clock::time_point timer_start, timer_stop;

            timer_start = std::chrono::high_resolution_clock::now();
            int idx = 0;
            while (idx < content_len)
            {
                size_t chunk_len = std::min((size_t)G_FILE_CHUNK_SIZE, content_len - idx);
                size_t enc_chunk_len = SharedSecretKey::get_cipher_len(chunk_len);
                uint8_t *enc_chunk_buf = new uint8_t[enc_chunk_len]{};

                data_key.encrypt(enc_chunk_buf, enc_chunk_len,
                                 &content_buf[idx], chunk_len,
                                 idx + chunk_len == content_len);

                DSComplete *notify = new DSComplete();
                notify->idx = idx;
                notify->cipher_len = enc_chunk_len;
                notify->cipher_buf = enc_chunk_buf;

                data_lock.lock();
                completion_queue.push(notify);
                data_lock.unlock();

                comp_q_smph.notify();

                idx += chunk_len;
            }

            timer_stop = std::chrono::high_resolution_clock::now();

            data_lock.lock();
            if (stopwatch)
            {
                *sym_enc_timer +=
                    std::chrono::duration_cast<std::chrono::milliseconds>(timer_stop - timer_start)
                        .count();
            }
            data_lock.unlock();
        };

        auto upload_lambda = [&]()
        {
            std::unique_lock<std::mutex> data_lock(g_data_mutex, std::defer_lock);
            std::chrono::high_resolution_clock::time_point timer_start, timer_stop;

            bool exit = false;

            while (!exit)
            {
                comp_q_smph.wait();

                timer_start = std::chrono::high_resolution_clock::now();

                data_lock.lock();
                auto comp_notify = completion_queue.front();
                completion_queue.pop();
                data_lock.unlock();

                // Upload this completed item to the storage
                UUID *enc_data_chunk_uuid = new UUID();
                int storage_conn = network_connect(get_storage_ip().c_str(), get_storage_port());
                upload_content(storage_conn, *enc_data_chunk_uuid,
                               comp_notify->cipher_buf, comp_notify->cipher_len, owner_keys);

                data_lock.lock();
                data_chunk_uuid_m[comp_notify->idx] = enc_data_chunk_uuid;
                exit = (data_chunk_uuid_m.size() == total_chunks);

                timer_stop = std::chrono::high_resolution_clock::now();
                if (stopwatch)
                {
                    *upload_timer += std::chrono::duration_cast<std::chrono::milliseconds>(timer_stop - timer_start).count();
                }
                data_lock.unlock();

                delete[] comp_notify->cipher_buf;
                delete comp_notify;
            }
        };

        std::thread enc_file_t(enc_lambda);
        std::thread upload_file_t(upload_lambda);

        enc_file_t.join();
        upload_file_t.join();

        // Clean up head usage
        if (!file_path.empty())
        {
            delete[] content_buf;
            content_buf = nullptr;
        }

        /**
         * Upload and protect data keys with Sieve encryption, one per owner
         */
        // Load device pubkey
        uint8_t device_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
        get_keyset().get_full_pk(device_pubkey, sizeof(device_pubkey));

        std::unordered_map<std::string, SharedSecretKey> owner_session_keys;
        std::unordered_map<std::string, SieveKey> owner_sieve_keys;
        std::unordered_map<std::string, int> owner_sockfd;

        // Generate random session ID
        uint8_t session_id[G_SESS_ID_SIZE]{};
        random_buf(session_id, sizeof(session_id));

        std::vector<std::thread *> fetch_sieve_cred_t;
        auto fetch_sieve_cred_lambda = [&](const uint8_t *owner_key)
        {
            std::unique_lock<std::mutex> data_lock(g_data_mutex, std::defer_lock);
            std::string owner_key_b64 = base64_encode(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

            // Fetch Sieve credential request to owner
            data_lock.lock();
            owner_session_keys[owner_key_b64] = SharedSecretKey();
            data_lock.unlock();

            CiphertextDataStoreSieveCredRequest request_payload;
            request_payload.type = CipherType::data_store_sieve_cred_request;
            memcpy(request_payload.session_key, owner_session_keys[owner_key_b64].get_key(), SharedSecretKey::KEY_SIZE);
            memcpy(request_payload.session_id, session_id, sizeof(request_payload.session_id));

            // Encrypt request payload with current owner's key
            size_t ciphertext_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(sizeof(request_payload));
            auto ciphertext_buf = new uint8_t[ciphertext_len]{0};
            uint8_t session_nonce[AsymmetricEncryptionKeySet::NONCE_SIZE]{0};
            get_keyset().box_easy(ciphertext_buf, ciphertext_len,
                                  reinterpret_cast<uint8_t *>(&request_payload),
                                  sizeof(request_payload), session_nonce, owner_key);

            flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
            auto device_pubkey_obj = builder.CreateVector(device_pubkey, sizeof(device_pubkey));
            auto session_nonce_obj = builder.CreateVector(session_nonce, sizeof(session_nonce));
            auto ciphertext_obj = builder.CreateVector(ciphertext_buf, ciphertext_len);
            auto sieve_request_msg = CreateDataStoreSieveCredRequest(builder,
                                                                     device_pubkey_obj,
                                                                     session_nonce_obj,
                                                                     ciphertext_obj);
            builder.Finish(sieve_request_msg);

            // Send out Sieve credential request_payload
            int owner_sock = connect_user_kms(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
            data_lock.lock();
            owner_sockfd[owner_key_b64] = owner_sock;
            data_lock.unlock();
            network_send_message_type(owner_sockfd[owner_key_b64], MessageType_DATA_STORE_SIEVE_CRED_REQUEST);
            network_send(owner_sockfd[owner_key_b64], builder.GetBufferPointer(), builder.GetSize());

            // Process Sieve credential response
            if (network_read_message_type(owner_sockfd[owner_key_b64]) != MessageType_DATA_STORE_SIEVE_CRED_RESPONSE)
            {
                LOGW("Unexpected Sieve credential response message type\n");
                return -1;
            }

            uint8_t response_buf[READ_BUFFER_SIZE]{0};
            network_read(owner_sockfd[owner_key_b64], response_buf, sizeof(response_buf));
            auto response_msg = GetDataStoreSieveCredResponse(response_buf);

            owner_session_keys[owner_key_b64].load_header_decryption(response_msg->session_header()->data(),
                                                                     response_msg->session_header()->size());

            CiphertextDataStoreSieveCredResponse response_payload;
            owner_session_keys[owner_key_b64].decrypt(reinterpret_cast<uint8_t *>(&response_payload), sizeof(response_payload),
                                                      response_msg->ciphertext()->data(), response_msg->ciphertext()->size());

            if (response_payload.type != CipherType::data_store_sieve_cred_response)
            {
                LOGW("Unexpected response type in encrypted_metadata\n");
                return -1;
            }

            data_lock.lock();
            owner_sieve_keys[owner_key_b64] = SieveKey(response_payload.sieve_key,
                                                       sizeof(response_payload.sieve_key),
                                                       response_payload.sieve_nonce, sizeof(response_payload.sieve_nonce));

            delete[] ciphertext_buf;
            return 0;
        };

        parent_timer_start = std::chrono::high_resolution_clock::now();
        for (auto &owner_key : owner_keys)
        {
            std::thread *ot = new std::thread(fetch_sieve_cred_lambda, owner_key);
            fetch_sieve_cred_t.push_back(ot);
        }

        for (auto t : fetch_sieve_cred_t)
        {
            t->join();
            delete t;
        }
        parent_timer_stop = std::chrono::high_resolution_clock::now();
        if (stopwatch)
        {
            *sieve_nego_timer +=
                std::chrono::duration_cast<std::chrono::milliseconds>(parent_timer_stop - parent_timer_start)
                    .count();
        }

        /*
            Assemble metadata block (the one that stores data key)
        */
        std::unordered_map<std::string, std::vector<uint8_t>> data_key_shares;
        split_key_shares(data_key, data_key_shares, owner_keys);

        std::unordered_map<std::string, UUID> encrypted_metadata_uuids;
        std::unordered_map<std::string, std::vector<int>> hints;

        std::vector<std::thread *> upload_meta_t;
        auto upload_meta_lambda = [&](int i)
        {
            auto owner_key = owner_keys[i];
            std::string owner_key_b64 = base64_encode(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

            std::unique_lock<std::mutex> data_lock(g_data_mutex, std::defer_lock);
            auto key_share = data_key_shares[owner_key_b64];
            MetadataBlock metadata;
            memcpy(metadata.data_key, &key_share[0], sizeof(metadata.data_key));

#if !defined(NDEBUG)
            LOGV("Original metadata:");
            hexprint(metadata.data_key, sizeof(metadata.data_key), 1);
#endif

            decaf::SecureBuffer encrypted_metadata;
            owner_sieve_keys[owner_key_b64].encrypt(reinterpret_cast<uint8_t *>(&metadata), sizeof(metadata),
                                                    hints[owner_key_b64], encrypted_metadata);

            data_lock.lock();
            encrypted_metadata_uuids[owner_key_b64] = UUID();
            data_lock.unlock();

            int storage_conn = network_connect(get_storage_ip().c_str(), get_storage_port());
            upload_content(storage_conn, encrypted_metadata_uuids[owner_key_b64],
                           &encrypted_metadata[0],
                           encrypted_metadata.size() * sizeof(encrypted_metadata[0]),
                           owner_keys);
        };

        parent_timer_start = std::chrono::high_resolution_clock::now();
        // Upload each user's metadata block (encrypted)
        for (int i = 0; i < owner_keys.size(); i++)
        {
            std::thread *ot = new std::thread(upload_meta_lambda, i);
            upload_meta_t.push_back(ot);
        }

        for (auto t : upload_meta_t)
        {
            t->join();
            delete t;
        }

        parent_timer_stop = std::chrono::high_resolution_clock::now();
        if (stopwatch)
        {
            *sieve_enc_timer +=
                std::chrono::duration_cast<std::chrono::milliseconds>(parent_timer_stop - parent_timer_start)
                    .count();
        }

        // Assemble Sieve data block (the plaintext data block)
        std::vector<flatbuffers::Offset<SieveDataBlock::OwnerInfo>> owners_v;
        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        for (auto &owner_key : owner_keys)
        {
            std::string owner_key_b64 = base64_encode(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

            auto sieve_nonce_obj = builder.CreateVector(owner_sieve_keys[owner_key_b64].get_nonce(), SIEVE_NONCE_SIZE);
            auto owner_pubkey_obj = builder.CreateVector(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
            auto encrypted_metadata_uuid_obj = builder.CreateString(encrypted_metadata_uuids[owner_key_b64].get_uuid());
            auto metadata_hint_obj = builder.CreateVector(hints[owner_key_b64]);

            auto owner_info_obj = SieveDataBlock::CreateOwnerInfo(builder,
                                                                  sieve_nonce_obj,
                                                                  owner_pubkey_obj,
                                                                  encrypted_metadata_uuid_obj,
                                                                  metadata_hint_obj);
            owners_v.push_back(owner_info_obj);
        }

        auto owners_v_obj = builder.CreateVector(owners_v);

        // Change data UUID into an array!!!!
        std::vector<flatbuffers::Offset<flatbuffers::String>> data_chunk_v;
        for (const auto &[idx, data_uuid] : data_chunk_uuid_m)
        {
            auto data_uuid_obj = builder.CreateString(data_uuid->get_uuid());
            data_chunk_v.push_back(data_uuid_obj);
            delete data_uuid;
        }
        auto data_uuid_v_obj = builder.CreateVector(data_chunk_v);
        auto data_header_obj = builder.CreateVector(data_key.get_header(), SharedSecretKey::HEADER_SIZE);
        auto sieve_data_block_msg = SieveDataBlock::CreateSieveDataBlock(builder, owners_v_obj,
                                                                         data_uuid_v_obj, data_header_obj);
        builder.Finish(sieve_data_block_msg);

        UUID sieve_data_block_uuid;

        parent_timer_start = std::chrono::high_resolution_clock::now();

        int storage_conn = network_connect(get_storage_ip().c_str(), get_storage_port());
        upload_content(storage_conn, sieve_data_block_uuid, builder.GetBufferPointer(), builder.GetSize(),
                       owner_keys);

        parent_timer_stop = std::chrono::high_resolution_clock::now();
        if (stopwatch)
        {
            *upload_timer += std::chrono::duration_cast<std::chrono::milliseconds>(parent_timer_stop - parent_timer_start).count();
        }

        /*
         Send back upload notification
         */
        std::vector<std::thread *> notify_t;
        auto notify_lambda = [&](const uint8_t *owner_key)
        {
            std::unique_lock<std::mutex> data_lock(g_data_mutex, std::defer_lock);
            std::string owner_key_b64 = base64_encode(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

            owner_session_keys[owner_key_b64].init_header_encryption(true);

            CiphertextDataStoreUploadNotification notification_payload;
            size_t notification_cipher_len = SharedSecretKey::get_cipher_len(sizeof(notification_payload));
            auto notification_cipher = new uint8_t[notification_cipher_len]{};

            notification_payload.type = CipherType::data_store_upload_notification;
            memcpy(notification_payload.sieve_data_block_uuid, sieve_data_block_uuid.get_uuid().c_str(),
                   UUID::UUID_SIZE);
            memcpy(notification_payload.session_id, session_id, sizeof(session_id));
            memcpy(notification_payload.encrypted_metadata_uuid, encrypted_metadata_uuids[owner_key_b64].get_uuid().c_str(),
                   UUID::UUID_SIZE);

            owner_session_keys[owner_key_b64].encrypt(notification_cipher,
                                                      notification_cipher_len,
                                                      reinterpret_cast<const unsigned char *>(&notification_payload),
                                                      sizeof(notification_payload));

            flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
            auto session_header_refresh_obj = builder.CreateVector(owner_session_keys[owner_key_b64].get_header(),
                                                                   SharedSecretKey::HEADER_SIZE);
            auto notification_cipher_obj = builder.CreateVector(notification_cipher, notification_cipher_len);
            auto notification_msg = DataStoreUpload::CreateDataStoreUploadNotification(builder,
                                                                                       session_header_refresh_obj,
                                                                                       notification_cipher_obj);
            builder.Finish(notification_msg);

            network_send_message_type(owner_sockfd[owner_key_b64], MessageType_DATA_STORE_UPLOAD_NOTIFICATION);
            network_send(owner_sockfd[owner_key_b64], builder.GetBufferPointer(), builder.GetSize());

            delete[] notification_cipher;
        };

        parent_timer_start = std::chrono::high_resolution_clock::now();

        for (auto &owner_key : owner_keys)
        {
            std::thread *ot = new std::thread(notify_lambda, owner_key);
            notify_t.push_back(ot);
        }
        for (auto t : notify_t)
        {
            t->join();
            delete t;
        }

        parent_timer_stop = std::chrono::high_resolution_clock::now();
        if (stopwatch)
        {
            *upload_notify_timer +=
                std::chrono::duration_cast<std::chrono::milliseconds>(parent_timer_stop - parent_timer_start)
                    .count();
        }

        if (sieve_block_result != nullptr)
        {
            *sieve_block_result = sieve_data_block_uuid;
        }

        return 0;
    }

    int Device::verify_certification(const uint8_t *m_ptr,
                                     size_t m_len,
                                     const uint8_t *s_ptr,
                                     size_t s_len)
    {
        // assert(s_len == AsymmetricEncryptionKeySet::get_signed_message_len(m_len));
        unsigned long long m_open_len = m_len;
        auto m_open = new uint8_t[m_open_len]{};

        for (auto owner : owner_keys)
        {
            int err = AsymmetricEncryptionKeySet::
                sign_verify_detached(s_ptr, s_len, m_ptr, m_len, owner);

            if (err == 0)
            {
                return 0;
            }
        }

        return -1;
    }

    int Device::encrypt_file_local(SharedSecretKey &data_key, const std::string &file_path, const std::string &encrypted_file_path)
    {
        std::ifstream file;
        file.open(file_path, std::ios::in | std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            LOGW("Failed to open file %s", file_path.c_str());
            return -1;
        }

        data_key.init_header_encryption();

        // Pre-allocate buffers
        char *data_buf = new char[G_DATA_BUF_SIZE]{};
        uint8_t *encrypted_data_buf = new uint8_t[SharedSecretKey::get_cipher_len(G_DATA_BUF_SIZE)]{};
        std::streampos file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        size_t processed = 0;
        // Flush output file by writing empty buffer
        write_to_file(encrypted_file_path, nullptr, 0);
        while (processed < file_size)
        {
            memset(data_buf, 0, G_DATA_BUF_SIZE);
            memset(encrypted_data_buf, 0, SharedSecretKey::get_cipher_len(G_DATA_BUF_SIZE));

            size_t chunk_len = std::min((size_t)G_DATA_BUF_SIZE, (size_t)file_size - processed);
            file.read(data_buf, chunk_len);
            processed += chunk_len;

            size_t encrypted_chunk_len = SharedSecretKey::get_cipher_len(chunk_len);
            data_key.encrypt(encrypted_data_buf, encrypted_chunk_len,
                             reinterpret_cast<const unsigned char *>(data_buf), chunk_len,
                             processed == file_size);

            write_to_file(encrypted_file_path, encrypted_data_buf,
                          encrypted_chunk_len, true);
        }

        file.close();

        delete[] encrypted_data_buf;
        delete[] data_buf;

        return 0;
    }

    int Device::encrypt_content_local(SharedSecretKey &data_key, const uint8_t *input_buf, size_t input_buf_len,
                                      uint8_t **encrypted_file_buf, size_t *encrypted_file_buf_len)
    {
        data_key.init_header_encryption();

        (*encrypted_file_buf_len) = SharedSecretKey::get_cipher_len(input_buf_len);
        (*encrypted_file_buf) = new uint8_t[*encrypted_file_buf_len]{};

        data_key.encrypt(*encrypted_file_buf, *encrypted_file_buf_len,
                         reinterpret_cast<const unsigned char *>(input_buf), input_buf_len);

        return 0;
    }

    bool Device::has_owner()
    {
        return owner_keys.size() != 0;
    }

    bool Device::real_time_access()
    {
        if (!has_owner())
        {
            LOGV("Empty device owners");
            return true;
        }

        auto fetch_owner_perm_lambda = [&](const uint8_t *owner_key, std::string owner_pk_b64)
        {
            if (real_time_perm.find(owner_pk_b64) == real_time_perm.end())
            {
                std::unique_lock<std::mutex> data_lock(g_data_mutex, std::defer_lock);

                // Load device pubkey
                uint8_t device_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
                get_keyset().get_full_pk(device_pubkey, sizeof(device_pubkey));

                // Need to ask this owner
                int owner_conn = connect_user_kms(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
                flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
                auto device_pubkey_obj = builder.CreateVector(device_pubkey, sizeof(device_pubkey));
                auto real_time_req_msg = CreateUtilRealTimeAccessRequest(builder,
                                                                         device_pubkey_obj);
                builder.Finish(real_time_req_msg);
                network_send_message_type(owner_conn, MessageType_UTIL_REAL_TIME_ACCESS_REQUEST);
                network_send(owner_conn, builder.GetBufferPointer(), builder.GetSize());

                if (network_read_message_type(owner_conn) != MessageType_UTIL_REAL_TIME_ACCESS_RESPONSE)
                {
                    LOGW("UNexpected real time access response type\n");
                    return -1;
                }

                uint8_t response_buf[READ_BUFFER_SIZE]{0};
                network_read(owner_conn, response_buf, sizeof(response_buf));
                auto response_msg = GetUtilRealTimeAccessResponse(response_buf);

                data_lock.lock();
                real_time_perm[owner_pk_b64] = response_msg->granted();
                data_lock.unlock();
            }
        };

        std::vector<std::thread *> fetch_owner_perm_t;
        for (auto &owner_key : owner_keys)
        {
            std::string owner_pk_b64 = base64_encode(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

            std::thread *ot = new std::thread(fetch_owner_perm_lambda, owner_key, owner_pk_b64);
            fetch_owner_perm_t.push_back(ot);
        }

        for (auto t : fetch_owner_perm_t)
        {
            t->join();
            delete t;
        }

        for (auto owner_pk : owner_keys)
        {
            std::string owner_pk_b64 = base64_encode(owner_pk, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
            if (!real_time_perm[owner_pk_b64])
            {
                LOGV("Owner %s denied.", owner_pk_b64.c_str());
                return false;
            }
        }

        LOGV("Default case");
        return true;
    }

    std::string Device::get_admin_key_b64()
    {
        return base64_encode(admin_key, sizeof(admin_key));
    }
}