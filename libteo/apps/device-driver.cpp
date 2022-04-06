// Partial code content referred from
// https://gist.github.com/syohex/b1aa695dc6ac5dced139

#include <libtot/libtot.hpp>
#include <fmt/format.h>
#include <argparse/argparse.hpp>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <pwd.h>
#include <chrono>
#include <thread>

// Custom httplib: https://github.com/yhirose/cpp-httplib
#include <httplib.h>

#include <nlohmann/json.hpp>

using std::cin;
using std::cout;
using std::endl;
using std::string;

using json = nlohmann::json;

typedef std::chrono::high_resolution_clock Clock;

static const size_t QR_CODE_WIDTH = 500;

static const char *driver_ip = "0.0.0.0";
static const int driver_port = 9020;

const string FILEPATH_KEY = "filePath";
const string FILECONTENT_KEY = "fileContent";
const string CERTIFICATE_CONTENT_KEY = "certContent";
const string CERTIFICATE_SIGNATURE_KEY = "certSign";
const string VERIFY_STATUS = "succeed";

const string SIEVE_ENC_TIME = "encryption_time_sieve";
const string SYM_ENC_TIME = "encryption_time_symmetric";
const string UPLOAD_TIME = "upload_time";
const string SIEVE_NEGOTIATION_TIME = "sieve_negotiation_time";
const string UPLOAD_NOTIFY_TIME = "upload_notification_time";
const string EXEC_TIME = "driver_execution_time";

const char *HTTP_CONTENT_TYPE_JSON = "application/json";
const char *HTTP_CONTENT_TYPE_RAW = "text/plain";

libtot::SharedSecretKey setup_token;

void *PrintDeviceDetail(void *dev_in)
{
    while (true)
    {
        libtot::Device *dev_ptr = (libtot::Device *)dev_in;

        string device_pubkey_b64 = libtot::base64_encode(dev_ptr->get_keyset().get_full_pk());

        // Test QR code
        string setup_token_b64 = libtot::base64_encode(setup_token.get_key(), libtot::SharedSecretKey::KEY_SIZE);
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

        fmt::print("==================================================================================================\n");

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

int main(int argc, char **argv)
{
    std::string storage_ip = "";
    int storage_port = 0;

    argparse::ArgumentParser program("TOT Device Driver");

    program.add_argument("storage_ip")
        .help("IP address of the storage provider.");

    program.add_argument("storage_port")
        .help("Port number of the storage provider.")
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

    fmt::print("Running Device Driver node...\n\n");

    libtot::api_initialize();

    setup_token = libtot::SharedSecretKey();
    libtot::Device dev(setup_token, storage_ip, storage_port);

    // Start task to periodically print setup details
    pthread_t thread_id;
    pthread_create(&thread_id, nullptr, PrintDeviceDetail, (void *)&dev);

    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request &, httplib::Response &res)
            { res.set_content("Hello World! Test end-point.", "text/plain"); });

    svr.Post("/store", [&](const httplib::Request &req, httplib::Response &res)
             {
                 fmt::print("{}\n", req.body);

                 auto req_json = json::parse(req.body);

                 libtot::UUID sieve_uuid_result;

                 int sieve_enc_timer = 0;
                 int sym_enc_timer = 0;
                 int upload_timer = 0;
                 int sieve_nego_timer = 0;
                 int upload_notify_timer = 0;

                 auto before = Clock::now();
                 if (req_json.find(FILEPATH_KEY) != req_json.end())
                 {
                     // Input is a file in the local file system
                     dev.store_data(req_json.at(FILEPATH_KEY), &sieve_uuid_result,
                                    &sieve_enc_timer, &sym_enc_timer, &upload_timer,
                                    &sieve_nego_timer, &upload_notify_timer);
                 }
                 else if (req_json.find(FILECONTENT_KEY) != req_json.end())
                 {
                     fmt::print("Processing direct input file content");
                     // Directly process user provided file content (in base64 encoding)
                     string content = req_json.at(FILECONTENT_KEY);
                     dev.store_data(reinterpret_cast<const uint8_t *>(content.c_str()), content.size(),
                                    &sieve_uuid_result, &sieve_enc_timer, &sym_enc_timer, &upload_timer);
                 }
                 else
                 {
                     fmt::print("Error processing request! Key not found.\n");
                     return;
                 }

                 auto after = Clock::now();
                 std::cout << "\tEncoding this request takes: "
                           << std::dec
                           << std::chrono::duration_cast<std::chrono::microseconds>(after - before).count()
                           << " microseconds" << std::endl;

                 std::cout << "\tData UUID: " << sieve_uuid_result.get_uuid() << std::endl;

                 json res_json;
                 res_json[SIEVE_ENC_TIME] = sieve_enc_timer;
                 res_json[SYM_ENC_TIME] = sym_enc_timer;
                 res_json[UPLOAD_TIME] = upload_timer;
                 res_json[SIEVE_NEGOTIATION_TIME] = sieve_nego_timer;
                 res_json[UPLOAD_NOTIFY_TIME] = upload_notify_timer;
                 res_json[EXEC_TIME] = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
                 res.set_content(res_json.dump(), HTTP_CONTENT_TYPE_JSON);
             });

    svr.Post("/verify", [&](const httplib::Request &req, httplib::Response &res)
             {
                 fmt::print("{}\n", req.body);
                 auto req_json = json::parse(req.body);

                 auto before = Clock::now();
                 if (req_json.find(CERTIFICATE_CONTENT_KEY) != req_json.end() &&
                     req_json.find(CERTIFICATE_SIGNATURE_KEY) != req_json.end())
                 {
                     // Process and validate user generated certificate
                     string cert_content_b64 = req_json.at(CERTIFICATE_CONTENT_KEY);
                     string cert_sign_b64 = req_json.at(CERTIFICATE_SIGNATURE_KEY);

                     // fmt::print("Cert content b64: {}\n", cert_content_b64);
                     // fmt::print("Cert sign b64: {}\n", cert_sign_b64);

                     string cert_content = libtot::base64_decode(cert_content_b64);
                     string cert_sign = libtot::base64_decode(cert_sign_b64);

                     // fmt::print("Cert content {}, length {}\n", cert_content, cert_content.size());
                     // fmt::print("Cert sign length {}\n", cert_sign.size());

                     int verify_res = dev.verify_certification(
                         reinterpret_cast<const uint8_t *>(cert_content.c_str()),
                         cert_content.size(),
                         reinterpret_cast<const uint8_t *>(cert_sign.c_str()),
                         cert_sign.size());

                     bool success = (verify_res == 0);
                     json res_json;
                     res_json[VERIFY_STATUS] = success;
                     res.set_content(res_json.dump(), HTTP_CONTENT_TYPE_JSON);
                 }
                 else
                 {
                     fmt::print("Error processing verify request! Key not found.\n");
                     return;
                 }
             });

    svr.Get("/access", [&](const httplib::Request &req, httplib::Response &res)
            {
                fmt::print("{}\n", req.body);
                bool success = false;

                // Ask TOT runtime to seek current owners' permissions
                success = dev.real_time_access();

                res.set_content(success ? "true" : "false", HTTP_CONTENT_TYPE_RAW);
            });

    svr.Get("/stop", [&](const httplib::Request &req, httplib::Response &res)
            { svr.stop(); });

    svr.listen(driver_ip, driver_port);

    return 0;
}
