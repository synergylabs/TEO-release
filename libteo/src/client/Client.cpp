//
// Created by han on 2/10/21.
//

#include <arpa/inet.h>
#include <cassert>
#include <cryptopp/ida.h>
#include <cryptopp/randpool.h>
#include <cryptopp/files.h>
#include <iostream>
#include <pthread.h>
#include <sys/socket.h>

#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "libtot/client/Client.hpp"
#include "libtot/libtot_network.hpp"
#include "libtot/libtot_logger.hpp"
#include "libtot/libtot_api.hpp"
#include "libtot/libtot_client_native.hpp"
#include "libtot/base64.hpp"

namespace libtot
{
    void SecretShareContent(int threshold, int nShares, const std::string &payload, std::vector<std::string> &res, const char *seed)
    {
        /*
        Source: https://stackoverflow.com/questions/20917558/how-to-use-shamir-secret-sharing-class-in-crypto
        */
        CryptoPP::RandomPool rng;
        if (seed != nullptr)
        {
            rng.IncorporateEntropy((CryptoPP::byte *)seed, strlen(seed));
        }

        CryptoPP::ChannelSwitch *channelSwitch;
        CryptoPP::StringSource source(payload, false, new CryptoPP::SecretSharing(rng, threshold, nShares, channelSwitch = new CryptoPP::ChannelSwitch));

        CryptoPP::vector_member_ptrs<CryptoPP::StringSink> strSinks(nShares);
        res.clear();
        res.resize(nShares);

        std::string channel;
        for (int i = 0; i < nShares; i++)
        {
            strSinks[i].reset(new CryptoPP::StringSink(res[i]));
            channel = CryptoPP::WordToString<CryptoPP::word32>(i);
            strSinks[i]->Put((CryptoPP::byte *)channel.data(), G_SECRET_SHARE_CHANNEL_ID_LEN);
            channelSwitch->AddRoute(channel, *strSinks[i], CryptoPP::DEFAULT_CHANNEL);
        }

        source.PumpAll();
    }

    void SecretRecoverContent(int threshold, std::string &result, const std::vector<std::string> &shares)
    {
        /*
        Source: https://stackoverflow.com/questions/20917558/how-to-use-shamir-secret-sharing-class-in-crypto
        */
        assert(shares.size() >= threshold);

        if (shares.size() <= 0)
        {
            return;
        }

        // CryptoPP::SecretRecovery recovery(threshold, new CryptoPP::FileSink(outFilename));
        CryptoPP::SecretRecovery recovery(threshold, new CryptoPP::StringSink(result));

        // CryptoPP::vector_member_ptrs<CryptoPP::FileSource> fileSources(threshold);
        CryptoPP::vector_member_ptrs<CryptoPP::StringSource> strSources(threshold);
        CryptoPP::SecByteBlock channel(4);
        int i;
        for (i = 0; i < threshold; i++)
        {
            strSources[i].reset(new CryptoPP::StringSource(shares[i], false));
            strSources[i]->Pump(G_SECRET_SHARE_CHANNEL_ID_LEN);
            strSources[i]->Get(channel, G_SECRET_SHARE_CHANNEL_ID_LEN);
            strSources[i]->Attach(new CryptoPP::ChannelSwitch(recovery, std::string((char *)channel.begin(),
                                                                                    G_SECRET_SHARE_CHANNEL_ID_LEN)));
        }

        while (strSources[0]->Pump(256))
            for (i = 1; i < threshold; i++)
                strSources[i]->Pump(256);

        for (i = 0; i < threshold; i++)
            strSources[i]->PumpAll();
    }

    void SecretShareFile(int threshold, int nShares, const char *filename, const char *seed)
    {
        /*
        Source: https://stackoverflow.com/questions/20917558/how-to-use-shamir-secret-sharing-class-in-crypto
        */
        CryptoPP::RandomPool rng;
        rng.IncorporateEntropy((CryptoPP::byte *)seed, strlen(seed));

        CryptoPP::ChannelSwitch *channelSwitch;
        CryptoPP::FileSource source(filename, false, new CryptoPP::SecretSharing(rng, threshold, nShares, channelSwitch = new CryptoPP::ChannelSwitch));

        CryptoPP::vector_member_ptrs<CryptoPP::FileSink> fileSinks(nShares);
        std::string channel;
        for (int i = 0; i < nShares; i++)
        {
            char extension[5] = ".000";
            extension[1] = '0' + CryptoPP::byte(i / 100);
            extension[2] = '0' + CryptoPP::byte((i / 10) % 10);
            extension[3] = '0' + CryptoPP::byte(i % 10);
            fileSinks[i].reset(new CryptoPP::FileSink((std::string(filename) + extension).c_str()));

            channel = CryptoPP::WordToString<CryptoPP::word32>(i);
            fileSinks[i]->Put((CryptoPP::byte *)channel.data(), 4);
            channelSwitch->AddRoute(channel, *fileSinks[i], CryptoPP::DEFAULT_CHANNEL);
        }

        source.PumpAll();
    }

    void SecretRecoverFile(int threshold, const char *outFilename, char *const *inFilenames)
    {
        /*
        Source: https://stackoverflow.com/questions/20917558/how-to-use-shamir-secret-sharing-class-in-crypto
        */
        CryptoPP::SecretRecovery recovery(threshold, new CryptoPP::FileSink(outFilename));

        CryptoPP::vector_member_ptrs<CryptoPP::FileSource> fileSources(threshold);
        CryptoPP::SecByteBlock channel(4);
        int i;
        for (i = 0; i < threshold; i++)
        {
            fileSources[i].reset(new CryptoPP::FileSource(inFilenames[i], false));
            fileSources[i]->Pump(4);
            fileSources[i]->Get(channel, 4);
            fileSources[i]->Attach(new CryptoPP::ChannelSwitch(recovery, std::string((char *)channel.begin(), 4)));
        }

        while (fileSources[0]->Pump(256))
            for (i = 1; i < threshold; i++)
                fileSources[i]->Pump(256);

        for (i = 0; i < threshold; i++)
            fileSources[i]->PumpAll();
    }

    // Initialize global client counter
    int Client::client_counter = 0;

    Client::Client()
    {
        assert(api_libtot_initialized());

        id = (client_counter + 1);
        client_counter = (client_counter + 1) % MAX_CLIENTS;

        keyset = AsymmetricEncryptionKeySet();

        calculate_message_type_flatbuffers_size();

#if !defined(NDEBUG)
        auto keypair_aead = keyset.get_aead_key();
        auto keypair_sign = keyset.get_sign_key();

        LOGV("Client %d AEAD public key:", id);
        hexprint(keypair_aead.get_pk(), keypair_aead.get_pk_size(), 1);

        LOGV("Client %d AEAD secret key:", id);
        hexprint(keypair_aead.get_sk(), keypair_aead.get_sk_size(), 1);

        LOGV("Client %d signature public key:", id);
        hexprint(keypair_sign.get_pk(), keypair_sign.get_pk_size(), 1);

        LOGV("Client %d signature secret key:", id);
        hexprint(keypair_sign.get_sk(), keypair_sign.get_sk_size(), 1);
#endif
    }

    Client::~Client() {}

    int Client::set_storage_info(const std::string &ip, int port)
    {
        storage_ip = ip;
        storage_port = port;

        return 0;
    }

    int Client::start_server()
    {
        // Create a socket
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            LOGE("Failed to create socket. errno: %d ", errno);
        }

        const int trueFlag = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
            LOGE("Failure");

        // Listen on the specified port for device module
        sockaddr_in sockaddr{};
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = INADDR_ANY;
        sockaddr.sin_port = htons(server_port);
        if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
        {
            LOGE("Failed to bind to port %d. errno: %d ", server_port, errno);
        }

        if (listen(sockfd, SERVER_MAX_CONNECTIONS) < 0)
        {
            LOGE("Failed to listen on socket. errno: %d", errno);
        }

        auto addrlen = sizeof(sockaddr);
        while (int connection = accept(sockfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen))
        {
            if (connection < 0)
            {
                LOGW("Failed to grab connection. errno: %d", errno);
                continue;
            }

            if (server_callback_handler(connection) != 0)
            {
                LOGW("Error handling this request!");
            }

            close(connection);
        }

        return 0;
    }

    void Client::set_server_ip(const std::string &ip_in)
    {
        server_ip = ip_in;
    }

    void Client::set_server_port(uint16_t port_in)
    {
        server_port = port_in;
    }

    std::string Client::get_server_ip()
    {
        // https://stackoverflow.com/questions/212528/get-the-ip-address-of-the-machine
        struct ifaddrs *ifAddrStruct = NULL;
        struct ifaddrs *ifa = NULL;
        void *tmpAddrPtr = NULL;

        getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (!ifa->ifa_addr)
            {
                continue;
            }
            if (ifa->ifa_addr->sa_family == AF_INET)
            { // check it is IP4
                // is a valid IP4 Address
                tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                // printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
                if (strcmp(ifa->ifa_name, "lo") != 0)
                {
                    // Get non-loopback interface
                    return std::string(addressBuffer);
                }
            }
            else if (ifa->ifa_addr->sa_family == AF_INET6)
            { // check it is IP6
                // Han: Skip. Don't care about IPv6

                // // is a valid IP6 Address
                // tmpAddrPtr = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                // char addressBuffer[INET6_ADDRSTRLEN];
                // inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                // printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
            }
        }
        if (ifAddrStruct != NULL)
            freeifaddrs(ifAddrStruct);

        return "";
    }

    uint16_t Client::get_server_port()
    {
        return server_port;
    }

    int Client::register_ip_kms()
    {

        uint8_t pk_buf[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
        get_keyset().get_full_pk(pk_buf, sizeof(pk_buf));

        return client_register_ip_kms_impl(pk_buf, sizeof(pk_buf),
                                           get_server_ip().c_str(), get_server_port(),
                                           storage_ip.c_str(), storage_port);

        // int conn = network_connect(storage_ip.c_str(), storage_port);

        // flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        // auto pubkey_obj = builder.CreateVector(pk_buf, sizeof(pk_buf));
        // auto ip_obj = builder.CreateString(server_ip);
        // auto register_msg = CreateUtilRegisterIp(builder, pubkey_obj, ip_obj, server_port);
        // builder.Finish(register_msg);

        // network_send_message_type(conn, MessageType_UTIL_REGISTER_IP);
        // network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        // return 0;
    }

    int Client::resolve_ip_kms(const uint8_t *pk, size_t pk_len, std::string &res_ip, int &res_port)
    {
        client_fetch_ip_kms_impl(pk, pk_len,
                                 get_storage_ip().c_str(), get_storage_port(),
                                 res_ip, res_port);

        return 0;
    }

    int Client::connect_user_kms(const uint8_t *pk, size_t pk_len)
    {
        std::string res_ip;
        int res_port;

        client_fetch_ip_kms_impl(pk, pk_len,
                                 storage_ip.c_str(), storage_port,
                                 res_ip, res_port);

        // FIXME:
        // Hack for missing User port
        if (res_port == 0)
        {
            res_port = 9010;
        }

        return network_connect(res_ip.c_str(), res_port);
    }

    int Client::split_key_shares(SharedSecretKey &data_key, std::unordered_map<std::string, std::vector<uint8_t>> &data_key_shares, const std::vector<const uint8_t *> &owner_keys)
    {
        std::string payload(reinterpret_cast<const char *>(data_key.get_key()), SharedSecretKey::KEY_SIZE);

        int n_shares = owner_keys.size();
        std::vector<std::string> shares;

        SecretShareContent(n_shares, n_shares, payload, shares);

        for (int i = 0; i < owner_keys.size(); i++)
        {
            auto k = owner_keys[i];
            auto k_b64 = base64_encode(k, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
            data_key_shares[k_b64] = std::vector<uint8_t>(shares[i].begin(), shares[i].end());
        }

        return 0;
    }

    int Client::assemble_key_shares(SharedSecretKey &data_key, std::unordered_map<std::string, std::vector<uint8_t>> &data_key_shares)
    {
        std::string payload;
        int n_shares = data_key_shares.size();
        std::vector<std::string> shares;

        for (auto kv : data_key_shares)
        {
            shares.push_back(std::string(reinterpret_cast<char *>(&kv.second[0]), kv.second.size()));
        }

        SecretRecoverContent(n_shares, payload, shares);

        data_key = SharedSecretKey(payload);

        return 0;
    }

    void *Client::server_wrapper(void *obj)
    {
        reinterpret_cast<Client *>(obj)->start_server();
        return nullptr;
    }

    void Client::wait_all()
    {
        void *status;

        LOGI("Client %d is waiting for all threads...", get_id());

        for (unsigned long thread : threads)
        {
            pthread_join(thread, &status);
        }
    }

    pthread_t *Client::get_thread(int thread_id)
    {
        if (thread_id < 0 || thread_id >= CLIENT_MAX_THREADS)
        {
            LOGE("Invalid thread id!");
        }
        return &threads[thread_id];
    }

    AsymmetricEncryptionKeySet &Client::get_keyset()
    {
        return keyset;
    }

}