#ifndef LIBTOT_STORAGE_HPP
#define LIBTOT_STORAGE_HPP

#include "leveldb/db.h"
#include <string>
#include <unordered_map>
#include <utility> // std::pair, std::make_pair
#include <mutex>

#include <libtot/Message.hpp>

namespace libtot
{
    class Storage : public Client
    {
    public:
        Storage();

        ~Storage();

        int server_callback_handler(int connection) override;

        int data_upload_handler(int connection);

        int data_download_handler(int connection);

        int data_reencrypt_handler(int connection);

        int respond_pubkey(int connection);

        int register_ip_port(int connection);

        int fetch_ip_port(int connection);

    private:
        static const int STORAGE_THREAD_SERVER = 0;
        std::unordered_map<std::string, std::pair<std::string, short>> kms;

        std::mutex g_db_mutex;

        leveldb::DB *content_db = nullptr;
        leveldb::DB *owner_db = nullptr;

        struct Query
        {
            leveldb::Status s;
            std::vector<std::string> owners_val;
            std::string content_val;
        };

        static bool compare_owner_keys(const std::vector<std::string> &owners_val, const flatbuffers::Vector<flatbuffers::Offset<libtot::DataStoreUpload::OwnerPubkey>> *owners_obj);

        bool db_initialized();

        Query fetch_db(const std::string &key);

        leveldb::Status write_db(const std::string &key, const std::vector<std::string> &owner_val_b64, const std::string &content_val_b64);
    };

    std::vector<std::string> construct_pk_str(const flatbuffers::Vector<flatbuffers::Offset<libtot::DataStoreUpload::OwnerPubkey>> *owners_obj);
}

#endif //LIBTOT_STORAGE_HPP