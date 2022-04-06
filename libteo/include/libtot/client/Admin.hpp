//
// Created by han on 2/11/21.
//

#ifndef LIBTOT_ADMIN_HPP
#define LIBTOT_ADMIN_HPP

#include <libtot/client/Client.hpp>
#include <libtot/SharedSecretKey.hpp>
#include <string>

namespace libtot
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
        int initialize_device(SharedSecretKey &setup_token, std::string device_pubkey_b64 = "");

        int pre_auth_handler(int connection);

        int server_callback_handler(int connection) override;
    };
}

#endif //LIBTOT_ADMIN_HPP
