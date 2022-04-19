//
// Created by han on 2/11/21.
//

#ifndef TEO_ADMIN_HPP
#define TEO_ADMIN_HPP

#include <teo/client/Client.hpp>
#include <teo/SharedSecretKey.hpp>
#include <string>

namespace teo
{
    class Admin : public Client
    {
    private:
        static const int ADMIN_THREAD_SERVER = 0;

    public:
        Admin(const std::string &storage_ip = default_device_ip,
              int storage_port = default_device_port);

        ~Admin();

        /**
         * Try to acquire a Device and become the manager.
         * @return
         */
        int initialize_device(SharedSecretKey &setup_key, std::string device_pubkey_b64 = "");

        int pre_auth_handler(int connection);

        int server_callback_handler(int connection) override;
    };
}

#endif //TEO_ADMIN_HPP
