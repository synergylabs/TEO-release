//
// Created by han on 2/11/21.
//

#include <cassert>
#include <errno.h>
#include <fcntl.h>
#include <flatbuffers/flatbuffers.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <sodium.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "teo/AsymmetricEncryptionKeySet.hpp"
#include "teo/client/Client.hpp"
#include "teo/teo_api.hpp"
#include "teo/teo_file_io.hpp"
#include "teo/teo_logger.hpp"
#include "teo/teo_params.hpp"
#include "teo/Message.hpp"

namespace teo
{
    int write_to_file(const std::string &output_file_path, const uint8_t *file_encrypted,
                      size_t file_encrypted_len, bool append)
    {
        std::ofstream encrypted_data_file;
        auto flags = std::ios::out | std::ios::binary;
        if (append)
        {
            flags = flags | std::ios::app;
        }
        encrypted_data_file.open(output_file_path, flags);

        if (!encrypted_data_file.is_open())
        {
            LOGW("Failed to open output file after encryption, path: %s", output_file_path.c_str());
            return -1;
        }

        encrypted_data_file.write(reinterpret_cast<const char *>(file_encrypted), file_encrypted_len);
        encrypted_data_file.close();

        return 0;
    }

    int read_from_file(const std::string &input_file_path, char **buf_ptr, size_t *buf_len_ptr)
    {
        std::ifstream data_file;
        data_file.open(input_file_path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!data_file.is_open())
        {
            LOGW("Error opening file %s", input_file_path.c_str());
            return -1;
        }
        std::streampos data_file_size = data_file.tellg();
        data_file.seekg(0, std::ios::beg);

        *buf_len_ptr = (size_t)data_file_size;
        (*buf_ptr) = new char[*buf_len_ptr];
        data_file.read(*buf_ptr, *buf_len_ptr);
        data_file.close();

        return 0;
    }

    int upload_file(int conn, const UUID &uuid, const std::string &file_path, std::vector<const uint8_t *> owner_keys)
    {
        // Open and read file
        std::ifstream file;
        file.open(file_path, std::ios::in | std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            LOGW("Failed to open file %s", file_path.c_str());
            return -1;
        }

        // Pre allocate buffers
        std::streampos file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto uuid_obj = builder.CreateString(uuid.get_uuid());

        std::vector<flatbuffers::Offset<DataStoreUpload::OwnerPubkey>> owner_key_v;
        for (auto &owner_key : owner_keys)
        {
            auto owner_key_pk_obj = builder.CreateVector(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
            auto owner_key_obj = DataStoreUpload::CreateOwnerPubkey(builder, owner_key_pk_obj);
            owner_key_v.push_back(owner_key_obj);
        }
        auto owner_key_v_obj = builder.CreateVector(owner_key_v);

        auto msg = DataStoreUpload::CreateDataStoreUpload(builder, uuid_obj, owner_key_v_obj, int32_t(file_size));
        builder.Finish(msg);

        network_send_message_type(conn, MessageType_DATA_STORE_UPLOAD);
        uint32_t total_size = builder.GetSize();
        total_size = htonl(total_size);
        send(conn, &total_size, sizeof(total_size), 0);
        network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(conn) != MessageType_DATA_STORE_UPLOAD_ACK)
        {
            LOGW("Fail to get storage node upload ack");
            return -1;
        }

        int fromfd = open(file_path.c_str(), O_RDONLY);
        sendfile(conn, fromfd, 0, size_t(file_size));
        // network_send(conn, reinterpret_cast<uint8_t*>(data_buf), size_t(file_size));

        file.close();
        close(fromfd);

        return 0;
    }

    int upload_content(int conn, const UUID &uuid, const uint8_t *data_buf, size_t data_buf_len, std::vector<const uint8_t *> owner_keys)
    {
        // Pre allocate buffers
        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto uuid_obj = builder.CreateString(uuid.get_uuid());

        std::vector<flatbuffers::Offset<DataStoreUpload::OwnerPubkey>> owner_key_v;
        for (auto &owner_key : owner_keys)
        {
            auto owner_key_pk_obj = builder.CreateVector(owner_key, AsymmetricEncryptionKeySet::FULL_PK_SIZE);
            auto owner_key_obj = DataStoreUpload::CreateOwnerPubkey(builder, owner_key_pk_obj);
            owner_key_v.push_back(owner_key_obj);
        }
        auto owner_key_v_obj = builder.CreateVector(owner_key_v);

        auto msg = DataStoreUpload::CreateDataStoreUpload(builder, uuid_obj, owner_key_v_obj, data_buf_len);
        builder.Finish(msg);

        network_send_message_type(conn, MessageType_DATA_STORE_UPLOAD);
        uint32_t total_size = builder.GetSize();
        total_size = htonl(total_size);
        send(conn, &total_size, sizeof(total_size), 0);
        network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(conn) != MessageType_DATA_STORE_UPLOAD_ACK)
        {
            LOGW("Fail to get storage node upload ack");
            return -1;
        }

        network_send(conn, data_buf, data_buf_len);

        return 0;
    }

    int download_file(int conn, const UUID &uuid, char **buf_ptr, size_t *buf_len_ptr)
    {
        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto uuid_obj = builder.CreateString(uuid.get_uuid());
        auto request_msg = DataStoreDownload::CreateDataStoreDownloadRequest(builder, uuid_obj);
        builder.Finish(request_msg);

        network_send_message_type(conn, MessageType_DATA_STORE_DOWNLOAD_REQUEST);
        network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(conn) != MessageType_DATA_STORE_DOWNLOAD_RESPONSE_1)
        {
            LOGW("Failed to get download response");
            return -1;
        }

        uint8_t res_buf_1[READ_BUFFER_SIZE]{0};
        network_read(conn, res_buf_1, sizeof(res_buf_1));

        network_send_message_type(conn, MessageType_DATA_STORE_DOWNLOAD_RESPONSE_1_ACK);
        auto res_msg_1 = DataStoreDownload::GetDataStoreDownloadResponse1(res_buf_1);
        *buf_len_ptr = res_msg_1->content_len();

        *buf_ptr = new char[*buf_len_ptr]{0};
        network_read(conn, reinterpret_cast<uint8_t *>(*buf_ptr), *buf_len_ptr, true);

        return 0;
    }
}