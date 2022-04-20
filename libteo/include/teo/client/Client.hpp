// Client is the base class of all TEO entities.
// Created by han on 2/11/21.
//

#ifndef TEO_CLIENT_HPP
#define TEO_CLIENT_HPP

#include <string>
#include <unordered_map>
#include <mutex>

#include "teo/AsymmetricEncryptionKeySet.hpp"
#include "teo/teo_network.hpp"
#include "teo/teo_params.hpp"
#include "teo/Message.hpp"
#include "teo/SharedSecretKey.hpp"

namespace teo
{
    void SecretShareContent(int threshold, int nShares, const std::string &payload, std::vector<std::string> &res, const char *seed = nullptr);
    void SecretRecoverContent(int threshold, std::string &result, const std::vector<std::string> &shares);

    class Client
    {
    private:
        static const int MAX_CLIENTS = 256;
        static const int SERVER_MAX_CONNECTIONS = 500;
        static const int CLIENT_MAX_THREADS = 10;

        static int client_counter;

        AsymmetricEncryptionKeySet keyset;

        int id = 0;
        std::string server_ip;
        uint16_t server_port = 0;

        std::string storage_ip = default_storage_ip;
        uint16_t storage_port = default_storage_port;

    public:
        pthread_t threads[CLIENT_MAX_THREADS] = {0};
        std::mutex g_data_mutex;

        Client();

        virtual ~Client();

        int get_id() const { return id; }

        std::string get_storage_ip() const { return storage_ip; };

        uint16_t get_storage_port() const { return storage_port; };

        int set_storage_info(const std::string &ip, int port);

        AsymmetricEncryptionKeySet &get_keyset();

        void set_server_ip(const std::string &ip_in);

        void set_server_port(uint16_t port_in);

        std::string get_server_ip();

        uint16_t get_server_port();

        int register_ip_kms();

        int resolve_ip_kms(const uint8_t *pk, size_t pk_len, std::string &res_ip, int &res_port);

        int connect_user_kms(const uint8_t *pk, size_t pk_len);

        static int split_key_shares(SharedSecretKey &data_key, std::unordered_map<std::string, std::vector<uint8_t>> &data_key_shares, const std::vector<const uint8_t *> &owner_keys);

        static int assemble_key_shares(SharedSecretKey &data_key, std::unordered_map<std::string, std::vector<uint8_t>> &data_key_shares);

        int start_server();

        static void *server_wrapper(void *obj);

        virtual int server_callback_handler(int connection) = 0;

        pthread_t *get_thread(int thread_id);

        /**
         * Wait for all threads to finish
         */
        virtual void wait_all();
    };

}

#endif //TEO_CLIENT_HPP
