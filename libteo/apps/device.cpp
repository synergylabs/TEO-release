// Partial code content referred from
// https://gist.github.com/syohex/b1aa695dc6ac5dced139

#include <teo/teo.hpp>
#include <fmt/format.h>
#include <argparse/argparse.hpp>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <pwd.h>
#include <chrono>
#include <thread>

using std::cin;
using std::cout;
using std::endl;
using std::string;

typedef std::chrono::high_resolution_clock Clock;

static const size_t QR_CODE_WIDTH = 500;

teo::SharedSecretKey setup_key;

void *PrintDeviceDetail(void *dev_in)
{
    while (true)
    {
        teo::Device *dev_ptr = (teo::Device *)dev_in;

        string device_pubkey_b64 = teo::base64_encode(dev_ptr->get_keyset().get_full_pk());

        // Test QR code
        string setup_token_b64 = teo::base64_encode(setup_key.get_key(), teo::SharedSecretKey::KEY_SIZE);
        string admin_qr_content_base = "{{ \"issuer\": \"device\", \"type\": \"admin\", \"DeviceSecret\": \"{}\", \"Pubkey\":\"{}\", \"IP\": \"{}\", \"Port\": \"{}\" }}";
        string admin_qr_content = fmt::format(admin_qr_content_base, setup_token_b64, device_pubkey_b64, dev_ptr->get_server_ip(), dev_ptr->get_server_port());

        string user_qr_content_base = "{{ \"issuer\": \"device\", \"type\": \"user\", \"Pubkey\":\"{}\", \"IP\": \"{}\", \"Port\": \"{}\", \"admin\": \"{}\" }}";
        string user_qr_content = fmt::format(user_qr_content_base, device_pubkey_b64, dev_ptr->get_server_ip(), dev_ptr->get_server_port(), dev_ptr->get_admin_key_b64());

        fmt::print("==================================================================================================\n");

        CURL *curl = curl_easy_init();
        if (curl)
        {
            char *admin_qr_output = curl_easy_escape(curl, admin_qr_content.c_str(), admin_qr_content.size());
            char *user_qr_output = curl_easy_escape(curl, user_qr_content.c_str(), user_qr_content.size());

            if (admin_qr_output && user_qr_output)
            {
                fmt::print("\nQR Code link for **admin**: https://api.qrserver.com/v1/create-qr-code/?size={}x{}&data={}\n\n",
                           QR_CODE_WIDTH, QR_CODE_WIDTH, admin_qr_output);
                fmt::print("Admin contents: {}\n", admin_qr_content.c_str());

                fmt::print("\nQR Code link for user: https://api.qrserver.com/v1/create-qr-code/?size={}x{}&data={}\n\n",
                           QR_CODE_WIDTH, QR_CODE_WIDTH, user_qr_output);
                fmt::print("User contents: {}\n", user_qr_content.c_str());

                curl_free(admin_qr_output);
                curl_free(user_qr_output);
            }
        }
        else
        {
            fmt::print("Error initiating CURL instance!");
        }

        fmt::print("Press any key to store data, or ENTER to stop the process\n");

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

int main()
{
    fmt::print("Running Device node...\n\n");

    teo::api_initialize();

    setup_key = teo::SharedSecretKey();
    teo::Device dev(setup_key);

    pthread_t thread_id;
    pthread_create(&thread_id, nullptr, PrintDeviceDetail, (void *)&dev);

    string action = "";
    while (getline(cin, action))
    {
        if (action.size() == 0)
        {
            fmt::print("Stopping...\n");
            break;
        }

        struct passwd *pw = getpwuid(getuid());
        std::string homedir = std::string(pw->pw_dir);

        auto before = Clock::now();
        std::string test_hello_world_path = homedir + "/teo/tests/hello-world.txt";
        teo::UUID sieve_uuid_hello_world;
        dev.store_data(test_hello_world_path, &sieve_uuid_hello_world);

        auto after = Clock::now();
        std::cout << "\tEncoding hello-world takes: "
                  << std::dec
                  << std::chrono::duration_cast<std::chrono::microseconds>(after - before).count()
                  << " microseconds" << std::endl;
    }

    return 0;
}