#include <arpa/inet.h>
#include <sys/socket.h>

#include "teo/teo_params.hpp"
#include "teo/teo_logger.hpp"
#include "teo/teo_network.hpp"

namespace teo
{

    // Decide flatbuffers encoded message type msg buffer size
    // for receiving.
    int message_type_flatbuffers_size = 0;

    int network_connect(const char *ip, unsigned int port)
    {
        int sockfd = 0;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            LOGW("Socket creation error. errno: %d", errno);
            return -1;
        }

        sockaddr_in device_addr{};
        device_addr.sin_family = AF_INET;
        device_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &device_addr.sin_addr) <= 0)
        {
            LOGW("Invalid address. Address not supported.");
            return -1;
        }
        if (connect(sockfd, (struct sockaddr *)&device_addr, sizeof(device_addr)) < 0)
        {
            LOGW("Connection failed.");
            return -1;
        }
        return sockfd;
    }

    int network_read(int sockfd, uint8_t *buf, size_t buf_len, bool wait_all)
    {
        size_t chunk_read = 0;
        // while (chunk_read == 0 || (wait_all && chunk_read < buf_len))
        // {
        size_t val_read = recv(sockfd, buf, buf_len, wait_all ? MSG_WAITALL : 0);
        if (val_read <= 0)
        {
            LOGW("Error reading from the socket");
            return -1;
        }
        chunk_read += val_read;
        // }
        return chunk_read;
    }

    int network_send(int sockfd, const uint8_t *buf, size_t buf_len, int flags, uint8_t *out)
    {
        int bytes_sent = 0;
        if (out == nullptr) {
            bytes_sent = send(sockfd, buf, buf_len, flags);
        } else {
            memcpy(out, buf, buf_len);
            bytes_sent = buf_len;
        }
        return bytes_sent;
    }

    int network_send_message_type(int sockfd, MessageType type, uint8_t *out)
    {
        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto msg = CreateMessageTypeMsg(builder, type);
        builder.Finish(msg);

        uint8_t *buf = builder.GetBufferPointer();
        size_t buf_len = builder.GetSize();

        int bytes_sent = 0;
        if (out == nullptr) {
            bytes_sent = send(sockfd, buf, buf_len, SOCKET_SEND_FLAGS);
        } else {
            memcpy(out, buf, buf_len);
            bytes_sent = buf_len;
        }

        return bytes_sent;
    }

    MessageType network_read_message_type(int sockfd)
    {
        MessageType res;

        if (message_type_flatbuffers_size == 0)
        {
            calculate_message_type_flatbuffers_size();
        }

        auto buf = new uint8_t[message_type_flatbuffers_size];
        network_read(sockfd, buf, message_type_flatbuffers_size);
        auto type_msg = GetMessageTypeMsg(buf);
        res = type_msg->type();

        delete[] buf;
        return res;
    }


    void calculate_message_type_flatbuffers_size()
    {
        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto msg = CreateMessageTypeMsg(builder, MessageType_MAX);
        builder.Finish(msg);

        builder.GetBufferPointer();
        message_type_flatbuffers_size = builder.GetSize();
    }

    int get_message_type_flatbuffers_size() {
        if (message_type_flatbuffers_size == 0) {
            calculate_message_type_flatbuffers_size();
        }
        return message_type_flatbuffers_size;
    }
}