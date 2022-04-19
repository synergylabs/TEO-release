#include <teo/teo.hpp>
#include <fmt/format.h>
#include <argparse/argparse.hpp>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <vector>

using std::cout;
using std::endl;
using std::string;

static const size_t QR_CODE_WIDTH = 500;
static const int USER_PORT_BASE = 9100;

int main(int argc, char *argv[])
{
    std::string storage_ip = "";
    int storage_port = 0;

    argparse::ArgumentParser program("TOT Simulate group of users");

    program.add_argument("storage_ip")
        .help("IP address of the storage provider.");

    program.add_argument("storage_port")
        .help("Port number of the storage provider.")
        .action([](const std::string &value)
                { return std::stoi(value); });

    program.add_argument("admin_pubkey")
        .help("Admin public key in base64 encoding.");

    program.add_argument("device_pubkey")
        .help("Device's public key in base64 encoding.");

    program.add_argument("group_size")
        .help("Number of users in the group for simulation")
        .action([](const std::string &value)
                { return std::stoi(value); });

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }

    storage_ip = program.get("storage_ip");
    storage_port = program.get<int>("storage_port");

    string admin_pubkey_b64 = program.get("admin_pubkey");
    string device_pubkey_b64 = program.get("device_pubkey");

    int group_size = program.get<int>("group_size");

    fmt::print("Start user groups v1.1\n");

    teo::api_initialize();

    std::vector<teo::User *> group;

    for (int i = 0; i < group_size; i++)
    {
        teo::User *user = new teo::User(reinterpret_cast<const uint8_t *>(teo::base64_decode(admin_pubkey_b64).c_str()),
                                              "0.0.0.0", USER_PORT_BASE + i,
                                              storage_ip, storage_port);

        user->acquire_pre_auth_token();
        user->claim_device(false, device_pubkey_b64);

        group.push_back(user);
    }

    for (teo::User *u : group)
    {
        u->wait_all();
    }

    return 0;
}