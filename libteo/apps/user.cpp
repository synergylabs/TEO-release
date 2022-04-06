#include <libtot/libtot.hpp>
#include <fmt/format.h>
#include <argparse/argparse.hpp>
#include <string>
#include <iostream>
#include <curl/curl.h>

using std::cout;
using std::endl;
using std::string;

static const size_t QR_CODE_WIDTH = 500;

int main(int argc, char *argv[])
{
    std::string storage_ip = "";
    int storage_port = 0;

    argparse::ArgumentParser program("TOT User");

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

    fmt::print("Running User node...\n\n");

    libtot::api_initialize();

    libtot::User user(reinterpret_cast<const uint8_t *>(libtot::base64_decode(admin_pubkey_b64).c_str()),
                      "0.0.0.0", 9011, storage_ip, storage_port);

    user.acquire_pre_auth_token();
    user.claim_device(false, device_pubkey_b64);

    fmt::print("Finish claiming\n");

    // Generate access certificate
    const char msg[] = "authorized user";
    size_t msg_len = sizeof(msg);
    uint8_t *cert = nullptr;
    size_t cert_len = 0;
    user.sign_access_cert(reinterpret_cast<const uint8_t *>(msg), msg_len, &cert, &cert_len);
    string msg_b64 = libtot::base64_encode(reinterpret_cast<const uint8_t *>(msg), msg_len);
    string cert_b64 = libtot::base64_encode(cert, cert_len);

    fmt::print("Msg content: {}\n", msg_b64);
    fmt::print("Cert content: {}\n", cert_b64);

    user.wait_all();

    return 0;
}