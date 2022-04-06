//
// Created by han on 2/10/21.
//
#include <libtot/libtot.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <libtot/client/Admin.hpp>

namespace libtot
{
    Admin::Admin(const std::string &storage_ip,
                       int storage_port)
    {
        set_server_port(default_admin_port);

        set_storage_info(storage_ip, storage_port);

        pthread_create(get_thread(ADMIN_THREAD_SERVER), nullptr, server_wrapper, this);

        register_ip_kms();

        LOGV("Client %d is Admin", get_id());
    }

    Admin::~Admin() {}

    int Admin::server_callback_handler(int connection)
    {
        MessageType incoming = network_read_message_type(connection);

        if (incoming == MessageType_ACQUIRE_PRE_AUTH_TOKEN_REQUEST)
        {
            return pre_auth_handler(connection);
        }

        return 0;
    }

    int Admin::initialize_device(SharedSecretKey &setup_token, std::string device_pubkey_b64)
    {
        std::string device_ip = default_device_ip;
        int device_port = default_device_port;

        if (!device_pubkey_b64.empty())
        {
            std::string device_pubkey = base64_decode(device_pubkey_b64);
            resolve_ip_kms(reinterpret_cast<const uint8_t*>(device_pubkey.c_str()), 
            device_pubkey.size(), device_ip, device_port);
        }

        return admin_initialize_device_impl(device_ip.c_str(), device_port,
                                            nullptr, AsymmetricEncryptionKeySet::FULL_PK_SIZE,
                                            setup_token, get_keyset());
    }

    int Admin::pre_auth_handler(int connection)
    {
        uint8_t request_buf[READ_BUFFER_SIZE];
        network_read(connection, request_buf, sizeof(request_buf));
        auto request_msg = GetAcquirePreAuthTokenRequest(request_buf);

        SignaturePreAuthToken token_content;
        memcpy(token_content.user_pubkey, request_msg->user_pubkey()->Data(), request_msg->user_pubkey()->size());

        CiphertextAcquirePreAuthTokenResponse response_payload;
        response_payload.type = CipherType::acquire_pre_auth_token;
        get_keyset().sign_detached(response_payload.token,
                                   reinterpret_cast<const unsigned char *>(&token_content),
                                   sizeof(token_content));

        LOGV("Event: Acquire pre auth token admin grant");
        LOGV("Token content base64: %s", base64_encode(response_payload.token, sizeof(response_payload.token)).c_str());

        uint8_t ciphertext_buf_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(sizeof(response_payload));
        auto ciphertext_buf = new uint8_t[ciphertext_buf_len];
        uint8_t nonce[AsymmetricEncryptionKeySet::NONCE_SIZE];
        get_keyset().box_easy(ciphertext_buf, ciphertext_buf_len, reinterpret_cast<const uint8_t *>(&response_payload),
                              sizeof(response_payload), nonce, request_msg->user_pubkey()->Data());

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto nonce_obj = builder.CreateVector(nonce, sizeof(nonce));
        auto ciphertext_obj = builder.CreateVector(ciphertext_buf, ciphertext_buf_len);
        auto response_msg = CreateAcquirePreAuthTokenResponse(builder, nonce_obj, ciphertext_obj);
        builder.Finish(response_msg);

        network_send_message_type(connection, MessageType_ACQUIRE_PRE_AUTH_TOKEN_RESPONSE);
        network_send(connection, builder.GetBufferPointer(), builder.GetSize());

        delete[] ciphertext_buf;

        return 0;
    }

}