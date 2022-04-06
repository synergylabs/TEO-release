//
// Created by han on 2/11/21.
//

#ifndef LIBTOT_DEVICE_HPP
#define LIBTOT_DEVICE_HPP

#include <vector>
#include <string>
#include <deque>
#include <unordered_map>

#include <libtot/SharedSecretKey.hpp>

namespace libtot
{

    class Device : public Client
    {
    private:
        static const int DEVICE_THREAD_SERVER = 0;
        static const int DEVICE_THREAD_INFO_PRITNER = 1;
        static const int DEVICE_THREAD_BEACON = 2;
        static const int DEVICE_THREAD_PROXIMITY_DETECT = 3;

        uint8_t valid_device_proof[AsymmetricEncryptionKeySet::SIGNATURE_SIZE]{0};
        SharedSecretKey setup_token;
        uint8_t admin_key[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};

        std::vector<const uint8_t *> owner_keys;
        std::unordered_map<std::string, bool> real_time_perm;
        int set_owner(uint8_t *pk, bool group_mode = false);
        void flush_owner_key();
        bool is_group();

        std::deque<std::string> beacon_msgs;
        std::unordered_map<std::string, int> owner_hb_count;

    public:
        Device();

        explicit Device(SharedSecretKey &setup_token_in,
                        const std::string &storage_ip = default_device_ip,
                        int storage_port = default_device_port);

        ~Device();

        static void *print_info_wrapper(void *obj);

        static void *beacon_wrapper(void *obj);

        int print_info();

        int launch_beacon();

        bool has_owner();

        int server_callback_handler(int connection) override;

        int accept_initialization_handler(int connection);

        int acquire_ownership_handler(int connection);

        int release_device_handler(int connection);

        int release_device_owner(std::string owner_pk_b64);

        int remove_real_time_access_handler(int connection);

        int process_heartbeat_handler(int connection);

        int store_data(const std::string &file_path,
                       UUID *sieve_block_result = nullptr,
                       int *sieve_enc_timer = nullptr,
                       int *sym_enc_timer = nullptr,
                       int *upload_timer = nullptr,
                       int *sieve_nego_timer = nullptr,
                       int *upload_notify_timer = nullptr);

        int store_data(const uint8_t *file_content_ptr,
                       size_t file_content_len,
                       UUID *sieve_block_result = nullptr,
                       int *sieve_enc_timer = nullptr,
                       int *sym_enc_timer = nullptr,
                       int *upload_timer = nullptr,
                       int *sieve_nego_timer = nullptr,
                       int *upload_notify_timer = nullptr);

        int store_data_tot_impl(UUID *sieve_block_result,
                                const std::string &file_path,
                                const uint8_t *input_buf,
                                size_t input_buf_len,
                                int *sieve_enc_timer,
                                int *sym_enc_timer,
                                int *upload_timer,
                                int *sieve_nego_timer,
                                int *upload_notify_timer);

        int verify_certification(const uint8_t *m_ptr,
                                 size_t m_len,
                                 const uint8_t *s_ptr,
                                 size_t s_len);

        int encrypt_file_local(SharedSecretKey &data_key, const std::string &file_path, const std::string &encrypted_file_path);

        int encrypt_content_local(SharedSecretKey &data_key, const uint8_t *input_buf, size_t input_buf_len,
                                  uint8_t **encrypted_file_buf, size_t *encrypted_file_buf_len);

        bool real_time_access();

        std::string get_admin_key_b64();
    };
}

#endif //LIBTOT_DEVICE_HPP
