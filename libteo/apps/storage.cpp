#include <libtot/libtot.hpp>
#include <libtot/client/Storage.hpp>

#include <chrono>
#include <fmt/format.h>
#include <iostream>
#include <pwd.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#include <curl/curl.h>
#include <thread>

using std::string;

using decaf::Block;
using decaf::SecureBuffer;
using decaf::SpongeRng;

typedef typename libtot::LIBTOT_EC_Group::Scalar Scalar;
typedef typename libtot::LIBTOT_EC_Group::Point Point;

typedef std::chrono::high_resolution_clock Clock;

static const size_t QR_CODE_WIDTH = 500;

void *PrintStorageDetail(void *store_in)
{
    while (true)
    {
        libtot::Storage *store_ptr = (libtot::Storage *)store_in;
        string store_pubkey_b64 = libtot::base64_encode(store_ptr->get_keyset().get_full_pk());

        string user_qr_content_base = "{{ \"issuer\": \"storage\", \"type\": \"user\", \"Pubkey\":\"{}\", \"IP\": \"{}\", \"Port\": \"{}\" }}";
        string user_qr_content = fmt::format(user_qr_content_base, store_pubkey_b64, store_ptr->get_server_ip(),
                                             store_ptr->get_server_port());

        fmt::print("==================================================================================================\n");

        CURL *curl = curl_easy_init();
        if (curl)
        {
            char *user_qr_output = curl_easy_escape(curl, user_qr_content.c_str(), user_qr_content.size());
            fmt::print("\nQR Code link for user: https://api.qrserver.com/v1/create-qr-code/?size={}x{}&data={}\n\n",
                       QR_CODE_WIDTH, QR_CODE_WIDTH, user_qr_output);
            fmt::print("User contents: {}\n", user_qr_content.c_str());

            curl_free(user_qr_output);
        }

        fmt::print("==================================================================================================\n");

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

int main()
{
    fmt::print("Running Storage node...\n\n");

    libtot::api_initialize();

    libtot::Storage store;

    pthread_t thread_id;
    pthread_create(&thread_id, nullptr, PrintStorageDetail, (void *)&store);

    store.wait_all();

    return 0;
}
