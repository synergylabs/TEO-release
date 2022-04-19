//
// Created by han on 3/31/21.
//
#ifndef TEO_ACCESSOR_HPP
#define TEO_ACCESSOR_HPP

#include <string>
#include <unordered_map>

#include <teo/Sieve.hpp>
#include <teo/teo_helper.hpp>
#include <teo/client/Client.hpp>

namespace teo
{
    class Accessor : public Client
    {
    private:
        std::unordered_map<teo::UUID, std::unordered_map<std::string, SieveKey>, UUIDHasher> sieve_key_cache;

    public:
        Accessor(const std::string &storage_ip = default_device_ip,
                 int storage_port = default_device_port);

        ~Accessor();

        int request_access(const teo::UUID &sieve_data_block_uuid,
                           std::string orig_file_path = "",
                           bool from_cache = false,
                           bool exp_fail = false,
                           int *sieve_dec_timer = nullptr,
                           int *sym_dec_timer = nullptr,
                           int *download_timer = nullptr);

        int server_callback_handler(int connection) override { return 0; }
    };
}

#endif //TEO_ACCESSOR_HPP
