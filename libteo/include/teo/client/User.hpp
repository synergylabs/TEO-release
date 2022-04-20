//
// Created by han on 2/19/21.
//

#ifndef TEO_USER_HPP
#define TEO_USER_HPP

#include <string>
#include <unordered_map>

#include <teo/teo_helper.hpp>
#include <teo/PreAuthToken.hpp>

namespace teo
{
    class User : public Client
    {
    private:
        static const int USER_THREAD_SERVER = 0;
        static const int MAX_RETRY = 10;

        const int SLEEP_INTERVAL = 5; // in milliseconds (ms)

        uint8_t admin_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE];
        std::string admin_ip = default_admin_ip;
        int admin_port = default_admin_port;

        PreAuthToken pre_auth_token;
        uint8_t claimed_device[AsymmetricEncryptionKeySet::FULL_PK_SIZE];

        std::unordered_map<UUID, SieveKey, UUIDHasher> sieve_data_key_lookup;
        std::unordered_map<UUID, UUID, UUIDHasher> metadata_sieve_lookup;
        std::unordered_map<UUID, UUID, UUIDHasher> sieve_metadata_lookup;


    public:
        explicit User(const uint8_t *admin_pubkey_in,
                      const std::string &user_ip,
                      short user_port,
                      const std::string &storage_ip = default_device_ip,
                      int storage_port = default_device_port);

        ~User();

        int acquire_pre_auth_token();

        int claim_device(bool exclusive = true, std::string device_pubkey_b64 = "");

        // int decrypt(const UUID &sieve_data_block_uuid, const std::string &orig_file_path);

        int re_encrypt(const UUID &sieve_data_block_uuid, const uint8_t *storage_pk = nullptr);

        bool delegate_access(const UUID &sieve_uuid, const uint8_t *request_pubkey, size_t request_pubkey_len);

        int data_store_handler(int connection);

        int data_access_handler(int connection);

        int server_callback_handler(int connection) override;

        int sign_access_cert(const uint8_t *msg, size_t msg_len, uint8_t **cert_ptr, size_t *cert_len_ptr);

        void wait_all() override;
    };
}

#endif //TEO_USER_HPP
