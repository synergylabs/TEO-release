//
// Created by han on 2/11/21.
//

#include <arpa/inet.h>

#include <libtot/libtot.hpp>
#include <libtot/client/Storage.hpp>

namespace libtot
{
    Storage::Storage()
    {
        LOGV("Client %d is Storage node", get_id());

        if (storage_content_location == storage_owner_location)
        {
            LOGE("Content database and owner database should be different");
        }

        // Open up LevelDB connection
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, storage_content_location, &content_db);
        if (!status.ok())
        {
            LOGE("Failed to open content database connection");
        }
        status = leveldb::DB::Open(options, storage_owner_location, &owner_db);
        if (!status.ok())
        {
            LOGE("Failed to open owner database connection");
        }

        set_server_port(default_storage_port);
        pthread_create(get_thread(STORAGE_THREAD_SERVER), nullptr,
                       server_wrapper, this);
    }

    Storage::~Storage()
    {
        // Close database connection
        delete content_db;
        content_db = nullptr;
        delete owner_db;
        owner_db = nullptr;
    }

    int Storage::server_callback_handler(int connection)
    {
        MessageType incoming = network_read_message_type(connection);

        switch (incoming)
        {
        case MessageType_DATA_STORE_UPLOAD:
            return data_upload_handler(connection);
            break;
        case MessageType_DATA_STORE_DOWNLOAD_REQUEST:
            return data_download_handler(connection);
            break;
        case MessageType_DATA_REENCRYPTION_PRE_REQUEST:
            return data_reencrypt_handler(connection);
            break;
        case MessageType_UTIL_FETCH_STORE_PUBKEY:
            return respond_pubkey(connection);
            break;
        case MessageType_UTIL_REGISTER_IP:
            return register_ip_port(connection);
            break;
        case MessageType_UTIL_FETCH_IP_REQUEST:
            return fetch_ip_port(connection);
            break;
        default:
            LOGW("Unknown message type!");
            break;
        }

        return 0;
    }

    std::vector<std::string> construct_pk_str(const flatbuffers::Vector<flatbuffers::Offset<libtot::DataStoreUpload::OwnerPubkey>> *owners_obj)
    {
        std::vector<std::string> res;
        for (int i = 0; i < owners_obj->size(); i++)
        {
            auto pk_obj = owners_obj->Get(i)->pk();
            res.push_back(std::string(reinterpret_cast<const char *>(pk_obj->data()),
                                      (size_t)pk_obj->size()));
        }
        return res;
    }

    bool Storage::compare_owner_keys(const std::vector<std::string> &owners_val, const flatbuffers::Vector<flatbuffers::Offset<libtot::DataStoreUpload::OwnerPubkey>> *owners_obj)
    {
        std::vector<std::string> owners_v_obj = construct_pk_str(owners_obj);
        bool eq = (owners_val.size() == owners_v_obj.size());

        for (int i = 0; i < owners_val.size() && eq; i++)
        {
            if (owners_val[i] != owners_v_obj[i])
            {
                eq = false;
            }
        }

        return eq;
    }

    bool Storage::db_initialized()
    {
        return content_db != nullptr && owner_db != nullptr;
    }

    int Storage::data_upload_handler(int connection)
    {
        assert(db_initialized());

        uint32_t total_size = 0;
        recv(connection, &total_size, sizeof(total_size), MSG_WAITALL);
        total_size = ntohl(total_size);

        LOGV("EXPECTING SIZE: %d", total_size);

        uint8_t request_buf[total_size]{};
        network_read(connection, request_buf, total_size, true);
        auto request_msg = DataStoreUpload::GetDataStoreUpload(request_buf);

        network_send_message_type(connection, MessageType_DATA_STORE_UPLOAD_ACK);

        // Check if key exists
        Query old_record = fetch_db(request_msg->uuid()->data());
        if (old_record.s.ok())
        {
            LOGI("Overwrite keys -- check owner match");
            if (!compare_owner_keys(old_record.owners_val, request_msg->owner_pubkeys()))
            {
                LOGE("Existing owner public key differs from request owner key");
            }
        }
        else if (!old_record.s.IsNotFound())
        {
            LOGE("Database query error.");
        }

        LOGV("owner pubkey len %d", request_msg->owner_pubkeys()->size());
        LOGV("content len %d", request_msg->content_len());

        std::string content;
        content.resize(request_msg->content_len());
        network_read(connection, reinterpret_cast<uint8_t *>(&content[0]), request_msg->content_len(), true);

        leveldb::Status s = write_db(request_msg->uuid()->data(),
                                     construct_pk_str(request_msg->owner_pubkeys()),
                                     content);

        return (s.ok() ? 0 : -1);
    }

    int Storage::data_download_handler(int connection)
    {
        assert(db_initialized());
        uint8_t request_buf[READ_BUFFER_SIZE]{};
        network_read(connection, request_buf, READ_BUFFER_SIZE);
        auto request_msg = DataStoreDownload::GetDataStoreDownloadRequest(request_buf);

        Query res = fetch_db(request_msg->uuid()->data());
        if (!res.s.ok())
        {
            LOGI("Invalid request. Key not exists");
            LOGD("Reason: %s", res.s.ToString().c_str());
            return -1;
        }

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE + res.owners_val.size() * AsymmetricEncryptionKeySet::FULL_PK_SIZE + res.content_val.size());

        std::vector<flatbuffers::Offset<libtot::DataStoreDownload::OwnerPubkey>> owner_pks_v;
        for (auto &pk : res.owners_val)
        {
            auto owner_pk_obj = builder.CreateVector(reinterpret_cast<const uint8_t *>(pk.c_str()), pk.size());
            auto owner_pubkey_obj = DataStoreDownload::CreateOwnerPubkey(builder, owner_pk_obj);
            owner_pks_v.push_back(owner_pubkey_obj);
        }
        auto owner_pks_obj = builder.CreateVector(owner_pks_v);
        auto response_msg = DataStoreDownload::CreateDataStoreDownloadResponse1(builder, owner_pks_obj, res.content_val.size());
        builder.Finish(response_msg);

        network_send_message_type(connection, MessageType_DATA_STORE_DOWNLOAD_RESPONSE_1);

        network_send(connection, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(connection) != MessageType_DATA_STORE_DOWNLOAD_RESPONSE_1_ACK)
        {
            LOGW("Fail to get accessor's ack");
            return -1;
        }

        LOGV("Download content len: %d", res.content_val.size());
        // LOGV("Content from storage: %s", base64_encode(res.content_val).c_str());
        network_send(connection,
                     reinterpret_cast<const uint8_t *>(res.content_val.c_str()), res.content_val.size());

        return 0;
    }

    int Storage::data_reencrypt_handler(int connection)
    {
        assert(db_initialized());

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        size_t cipher_len = 0;
        uint8_t *cipher = nullptr;

        // Process pre request
        uint8_t pre_req_buf[READ_BUFFER_SIZE]{};
        network_read(connection, pre_req_buf, sizeof(pre_req_buf));
        auto pre_req_msg = GetDataReencryptionPreRequest(pre_req_buf);
        CiphertextDataReencryptionPreRequest pre_req_payload;
        get_keyset().box_seal_open(reinterpret_cast<uint8_t *>(&pre_req_payload),
                                   sizeof(pre_req_payload),
                                   pre_req_msg->ciphertext()->data(),
                                   pre_req_msg->ciphertext()->size());
        if (pre_req_payload.type != CipherType::data_reencryption_pre_request)
        {
            LOGW("Unexpected ciphertext payloadtype for pre-request");
            return -1;
        }

        // Load Sieve data block
        Storage::Query record = fetch_db(std::string(reinterpret_cast<char *>(&pre_req_payload.sieve_data_block_uuid),
                                                     sizeof(pre_req_payload.sieve_data_block_uuid)));
        if (!record.s.ok())
        {
            LOGW("Record not found");
            return -1;
        }
        auto sieve_data_content = SieveDataBlock::GetSieveDataBlock(record.content_val.c_str());
        const SieveDataBlock::OwnerInfo *owner_ptr = nullptr;
        for (int i = 0; i < sieve_data_content->owners()->size(); i++)
        {
            auto owner_info = sieve_data_content->owners()->Get(i);
            if (memcmp(owner_info->encrypted_metadata_uuid()->data(),
                       pre_req_payload.encrypted_metadata_uuid,
                       sizeof(pre_req_payload.encrypted_metadata_uuid)) == 0)
            {
                // Found enc metadata block, update owner information
                owner_ptr = owner_info;
                break;
            }
        }

        if (owner_ptr == nullptr)
        {
            LOGW("Owner not found based on Enc Meta UUID");
            return -1;
        }

        // Generate pre response
        CiphertextDataReencryptionPreResponse pre_res_payload;
        pre_res_payload.type = CipherType::data_reencryption_pre_response;
        memcpy(pre_res_payload.user_nonce,
               pre_req_payload.user_nonce,
               sizeof(pre_res_payload.user_nonce));
        random_buf(pre_res_payload.storage_nonce,
                   sizeof(pre_res_payload.storage_nonce));

        cipher_len = AsymmetricEncryptionKeySet::get_box_seal_cipher_len(sizeof(pre_res_payload));
        delete[] cipher;
        cipher = new uint8_t[cipher_len]{0};
        get_keyset().box_seal(cipher,
                              cipher_len,
                              reinterpret_cast<const uint8_t *>(&pre_res_payload),
                              sizeof(pre_res_payload),
                              owner_ptr->owner_pubkey()->data());
        builder.Clear();
        auto pre_res_cipher_obj = builder.CreateVector(cipher, cipher_len);
        auto pre_res_msg = CreateDataReencryptionPreResponse(builder, pre_res_cipher_obj);
        builder.Finish(pre_res_msg);

        network_send_message_type(connection, MessageType_DATA_REENCRYPTION_PRE_RESPONSE);
        network_send(connection, builder.GetBufferPointer(), builder.GetSize());

        // Proces the main request body
        if (network_read_message_type(connection) != MessageType_DATA_REENCRYPTION_REQUEST)
        {
            LOGW("Unexpected message type for reencryption main request");
            return -1;
        }
        uint8_t request_buf[READ_BUFFER_SIZE]{};
        network_read(connection, request_buf, READ_BUFFER_SIZE);
        auto request_msg = GetDataReencryptionRequest(request_buf);

        CiphertextDataReencryptionRequest request_payload;
        LOGV("Storage side:");

        get_keyset().box_open_easy(reinterpret_cast<uint8_t *>(&request_payload),
                                   sizeof(request_payload),
                                   request_msg->ciphertext()->data(),
                                   request_msg->ciphertext()->size(),
                                   request_msg->msg_nonce()->data(),
                                   owner_ptr->owner_pubkey()->data());
        if (request_payload.type != CipherType::data_reencryption_request)
        {
            LOGW("Incorrect request type");
            return -1;
        }

        std::vector<int> hints;
        for (int i = 0; i < owner_ptr->metadata_hint()->size(); i++)
        {
            hints.push_back(owner_ptr->metadata_hint()->Get(i));
        }

        UUID metadata_block_uuid = UUID(owner_ptr->encrypted_metadata_uuid()->data(),
                                        owner_ptr->encrypted_metadata_uuid()->size());

        Storage::Query metadata_record = fetch_db(metadata_block_uuid.get_uuid());
        if (!metadata_record.s.ok())
        {
            LOGW("Failed to fetch metadata");
            return -1;
        }

        SieveKey::apply_rekey_token_cipher(reinterpret_cast<uint8_t *>(&(metadata_record.content_val[0])),
                                           metadata_record.content_val.size(),
                                           hints,
                                           request_payload.rekey_token,
                                           owner_ptr->sieve_nonce()->data());

        std::vector<std::string> owner_pk_v = {
            std::string(reinterpret_cast<const char *>(owner_ptr->owner_pubkey()->data()),
                        owner_ptr->owner_pubkey()->size())};
        leveldb::Status s = write_db(metadata_block_uuid.get_uuid(),
                                     owner_pk_v,
                                     metadata_record.content_val);

        if (!s.ok())
        {
            LOGW("Error inserting new record in the database");
            return -1;
        }

        // Send back notification
        builder.Clear();
        auto notify_token_obj = builder.CreateVector(request_payload.noti_token,
                                                     sizeof(request_payload.noti_token));
        auto res_msg = CreateDataReencryptionResponse(builder, notify_token_obj);
        builder.Finish(res_msg);

        network_send_message_type(connection, MessageType_DATA_REENCRYPTION_RESPONSE);
        network_send(connection, builder.GetBufferPointer(), builder.GetSize());

        delete[] cipher;

        return 0;
    }

    Storage::Query Storage::fetch_db(const std::string &key)
    {
        Storage::Query res;
        std::string owners;
        std::unique_lock<std::mutex> data_lock(g_db_mutex, std::defer_lock);

        data_lock.lock();
        res.s = owner_db->Get(leveldb::ReadOptions(), key, &owners);
        data_lock.unlock();

        if (!res.s.ok())
        {
            return res;
        }

        // Split up list of owners
        const int LEN = AsymmetricEncryptionKeySet::FULL_PK_SIZE;
        int total = owners.size() / LEN;
        for (int i = 0; i < total; i++)
        {
            res.owners_val.push_back(owners.substr(i * LEN, LEN));
        }

        data_lock.lock();
        res.s = content_db->Get(leveldb::ReadOptions(), key, &res.content_val);
        data_lock.unlock();

        if (!res.s.ok())
        {
            LOGE("Key exists in owner but not in content! Data base inconsistent.");
        }

        LOGV("Read content for UUID: %s", key.c_str());

        return res;
    }

    leveldb::Status Storage::write_db(const std::string &key, const std::vector<std::string> &owners_val, const std::string &content_val)
    {
        leveldb::Status s;
        std::unique_lock<std::mutex> data_lock(g_db_mutex, std::defer_lock);

        data_lock.lock();
        s = content_db->Put(leveldb::WriteOptions(), key, content_val);
        data_lock.unlock();

        LOGV("Write UUID: %s to content db", key.c_str());
        if (!s.ok())
        {
            return s;
        }

        std::string owner_val;
        for (auto &o : owners_val)
        {
            owner_val.append(o);
        }

        data_lock.lock();
        s = owner_db->Put(leveldb::WriteOptions(), key, owner_val);
        data_lock.unlock();

        LOGV("Write UUID: %s to owner db", key.c_str());
        LOGV("owner value: ");
        hexprint(reinterpret_cast<const uint8_t *>(owner_val.c_str()), owner_val.size());

        if (!s.ok())
        {
            // Rollback on content database
            data_lock.lock();
            content_db->Delete(leveldb::WriteOptions(), key);
            data_lock.unlock();

            LOGV("Rollback database for UUID key: %s", key.c_str());
        }
        return s;
    }

    int Storage::respond_pubkey(int connection)
    {
        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        uint8_t pk_buf[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
        get_keyset().get_full_pk(pk_buf, sizeof(pk_buf));
        auto pk_obj = builder.CreateVector(pk_buf, sizeof(pk_buf));
        auto msg = CreateUtilFetchStorePubkey(builder, pk_obj);
        builder.Finish(msg);
        network_send(connection, builder.GetBufferPointer(), builder.GetSize());
        return 0;
    }

    int Storage::register_ip_port(int connection)
    {
        uint8_t buf[G_DATA_BUF_SIZE]{};
        network_read(connection, buf, sizeof(buf));
        auto req = GetUtilRegisterIp(buf);

        auto key = base64_encode(req->pubkey()->data(), req->pubkey()->size());
        auto val = std::make_pair(req->ip()->data(), req->port());
        if (kms.find(key) != kms.end())
        {
            LOGW("KMS registration key already exist!");
            LOGW("Overwritting...");
        }

        kms[key] = val;
        LOGV("Written new ip and port for %s", key.c_str());

        return 0;
    }

    int Storage::fetch_ip_port(int connection)
    {
        uint8_t buf[G_DATA_BUF_SIZE]{};
        network_read(connection, buf, sizeof(buf));
        auto req = GetUtilFetchIpRequest(buf);

        auto key = base64_encode(req->pubkey()->data(), req->pubkey()->size());
        if (kms.find(key) == kms.end())
        {
            LOGW("Requested pubkey not found in KMS!");
            return -1;
        }

        auto val = kms[key];

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto ip_obj = builder.CreateString(val.first);
        auto res_msg = CreateUtilFetchIpResponse(builder, ip_obj, val.second);
        builder.Finish(res_msg);

        network_send_message_type(connection, MessageType_UTIL_FETCH_IP_RESPONSE);
        network_send(connection, builder.GetBufferPointer(), builder.GetSize());

        return 0;
    }
}