#include <teo/teo.hpp>

#include <chrono>
#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <pwd.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <argparse/argparse.hpp>

using std::string;

using decaf::Block;
using decaf::SecureBuffer;
using decaf::SpongeRng;

typedef typename teo::TEO_EC_Group::Scalar Scalar;
typedef typename teo::TEO_EC_Group::Point Point;

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("TOT main test");

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

    std::string storage_ip = program.get("storage_ip");
    int storage_port = program.get<int>("storage_port");

    teo::api_initialize();

    // Testing secret sharing
    fmt::print("\n\nTesting Shamir Secret Sharing\n");

    std::string content;

    for (int i = 0; i < 256; i++)
    {
        content.push_back(char(i % 128));
    }

    std::cout << "Original content: " << std::endl;
    std::cout << content.size() << std::endl;
    teo::hexprint(content.c_str(), content.size());
    std::cout << std::endl;

    std::vector<std::string> secret_shares;
    int thresh = 2;
    teo::SecretShareContent(thresh, 3, content, secret_shares, "0");

    for (auto &s : secret_shares)
    {
        std::cout << s.size() << std::endl;
        teo::hexprint(s.c_str(), s.size());
    }

    std::string recovery;
    teo::SecretRecoverContent(thresh, recovery, secret_shares);
    std::cout << "Recovery: " << std::endl;
    std::cout << recovery.size() << std::endl;
    teo::hexprint(recovery.c_str(), recovery.size());
    std::cout << std::endl;

    // Test constructing elliptic curve cryptography
    fmt::print("\nTesting elliptic curve cryptography...\n");
    teo::test_ec();

    fmt::print("\nTesting Sieve crypto...\n");
    string x = "abc";

    Scalar k = teo::sieve_keygen();
    SecureBuffer fkx = teo::test_sieve_prf(k, x.c_str());
    teo::hexprint("fkx", fkx);

    SecureBuffer fNkx = teo::test_sieve_prf(-k, x.c_str());
    teo::hexprint("f(-k)x", fNkx);

    fmt::print("\n");

    Point fkxp(fkx), fNkxp(fNkx);
    teo::print("fkxp", fkxp);
    teo::print("fNkxp", fNkxp);
    teo::print("two point sum", fkxp + fNkxp);

    fmt::print("\n");

    string message = "hello";
    teo::test_sieve_encrypt_block(message, fkxp);

    message = "abc def";
    teo::test_sieve_encrypt_block(message, fkxp);

    message = "hello";
    teo::test_sieve_encrypt_block(message, fkxp);

    /**
     * TOT API tests
     */

    fmt::print("\n\nTesting public facing APIs\n");

    teo::SharedSecretKey setup_key = teo::SharedSecretKey();

    teo::Device dev(setup_key, storage_ip, storage_port);

    teo::Admin admin(storage_ip, storage_port);

    uint8_t admin_pubkey[teo::AsymmetricEncryptionKeySet::FULL_PK_SIZE];
    admin.get_keyset().get_full_pk(admin_pubkey, sizeof(admin_pubkey));
    teo::User user(admin_pubkey, "127.0.0.1", 9010, storage_ip, storage_port);

    teo::Accessor acc(storage_ip, storage_port);

    sleep(2);
    fmt::print("\nInitialize device...\n");

    admin.initialize_device(setup_key);

    sleep(2);
    fmt::print("\nAcquire pre auth token...\n");

    user.acquire_pre_auth_token();

    sleep(2);
    fmt::print("\nClaim device...\n");

    user.claim_device();

    sleep(2);
    fmt::print("\nTest encryptions...\n");

    auto before = Clock::now();
    std::string test_hello_world_path = std::filesystem::current_path().string() + "/../../tests/hello-world.txt";
    teo::UUID sieve_uuid_hello_world;
    dev.store_data(test_hello_world_path, &sieve_uuid_hello_world);

    auto after = Clock::now();
    std::cout << "Encoding hello-world takes: "
              << std::dec
              << std::chrono::duration_cast<std::chrono::microseconds>(after - before).count()
              << " microseconds" << std::endl;

    sleep(2);
    fmt::print("\n");

    before = Clock::now();
    std::string test_big_path = std::filesystem::current_path().string() + "/../../tests/big-10MB.txt";
    teo::UUID sieve_uuid_big;
    dev.store_data(test_big_path, &sieve_uuid_big);
    after = Clock::now();
    std::cout << "Encoding bit-10MB takes: "
              << std::dec
              << std::chrono::duration_cast<std::chrono::microseconds>(after - before).count()
              << " microseconds" << std::endl;

    sleep(2);
    fmt::print("\nTest requesting data access...\n");

    acc.request_access(sieve_uuid_hello_world, test_hello_world_path);

    sleep(2);
    fmt::print("\nTest re-encryption...\n");

    user.re_encrypt(sieve_uuid_hello_world);
    acc.request_access(sieve_uuid_hello_world, test_hello_world_path);

    sleep(2);
    fmt::print("\nTest group membership...\n");

    teo::User companion(admin_pubkey, "127.0.0.1", 9011, storage_ip, storage_port);
    companion.acquire_pre_auth_token();
    companion.claim_device(false);

    dev.store_data(test_hello_world_path, &sieve_uuid_hello_world);
    acc.request_access(sieve_uuid_hello_world, test_hello_world_path);

    sleep(2);
    fmt::print("\nTest group membership re-encryption...\n");

    companion.re_encrypt(sieve_uuid_hello_world);
    acc.request_access(sieve_uuid_hello_world, test_hello_world_path);

    fmt::print("\nTest accessor using old key after another re-encryption... should fail...\n");
    user.re_encrypt(sieve_uuid_hello_world);
    acc.request_access(sieve_uuid_hello_world, test_hello_world_path, true, true);

    sleep(2);
    fmt::print("\n");

    // dev.wait_all();

    return 0;
}
