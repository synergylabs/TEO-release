#ifndef LIBTOT_LIBTOT_NETWORK_HPP
#define LIBTOT_LIBTOT_NETWORK_HPP

#include "Message.hpp"
#include "libtot_params.hpp"

namespace libtot {
    extern int message_type_flatbuffers_size;

    void calculate_message_type_flatbuffers_size();

    int get_message_type_flatbuffers_size();

    int network_connect(const char *ip, unsigned int port);

    int network_send(int sockfd, const uint8_t *buf, size_t buf_len, int flags = SOCKET_SEND_FLAGS, uint8_t *out = nullptr);

    int network_read(int sockfd, uint8_t *buf, size_t buf_len, bool wait_all = false);

    int network_send_message_type(int sockfd, MessageType type, uint8_t *out = nullptr);

    MessageType network_read_message_type(int sockfd);
}

#endif // LIBTOT_LIBTOT_NETWORK_HPP
