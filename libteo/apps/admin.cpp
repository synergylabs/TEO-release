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
    argparse::ArgumentParser program("TOT Admin");

    program.add_argument("storage_ip")
        .help("IP address of the storage provider.");

    program.add_argument("storage_port")
        .help("Port number of the storage provider.")
        .action([](const std::string &value)
                { return std::stoi(value); });

    program.add_argument("device_pubkey")
        .help("Device public key in base64 encoding.");

    program.add_argument("device_secret")
        .help("Pre-shared device secrets for target device.");

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

    std::string storage_ip = program.get("storage_ip");
    int storage_port = program.get<int>("storage_port");

    string device_secret_b64_str = program.get("device_secret");
    string device_pubkey_b64 = program.get("device_pubkey");

    fmt::print("Running Admin node...\n\n");

    libtot::api_initialize();

    libtot::SharedSecretKey device_secret(libtot::base64_decode(device_secret_b64_str));

    libtot::Admin admin(storage_ip, storage_port);

    admin.initialize_device(device_secret, device_pubkey_b64);

    string admin_pubkey_b64 = libtot::base64_encode(admin.get_keyset().get_full_pk());
    string user_qr_content_base = "{{ \"issuer\": \"admin\", \"type\": \"user\", \"Pubkey\":\"{}\", \"IP\": \"{}\", \"Port\": \"{}\" }}";
    string user_qr_content = fmt::format(user_qr_content_base, admin_pubkey_b64, admin.get_server_ip(), admin.get_server_port());

    CURL *curl = curl_easy_init();
    if (curl)
    {
        char *user_qr_output = curl_easy_escape(curl, user_qr_content.c_str(), user_qr_content.size());

        fmt::print("\nQR Code link for **user**: https://api.qrserver.com/v1/create-qr-code/?size={}x{}&data={}\n\n",
                   QR_CODE_WIDTH, QR_CODE_WIDTH, user_qr_output);
        fmt::print("User contents: {}\n", user_qr_content.c_str());

        curl_free(user_qr_output);
    }

    admin.wait_all();

    return 0;
}