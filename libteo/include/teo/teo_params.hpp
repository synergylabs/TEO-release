//
// Created by han on 2/11/21.
//

#ifndef TEO_TEO_PARAMS_HPP
#define TEO_TEO_PARAMS_HPP

#include <decaf.hxx>

namespace teo
{
    /**
     * EC group performance:
     *      Ristretto/Ed25519: ~0.28ms per operation
     *      Ed448Goldilocks: ~0.4ms per operation
     */
    //    typedef typename decaf::Ristretto TEO_EC_Group;
    typedef typename decaf::Ed448Goldilocks TEO_EC_Group;

    const int G_CHALLENGE_SIZE = 32;
    const int G_DATA_BUF_SIZE = 40960;
    const int G_FBS_SIZE = 40960;
    const int G_SESS_ID_SIZE = 16;
    const int G_SECRET_SHARE_CHANNEL_ID_LEN = 4; // Size of Secret Sharing Channel length. 4 bytes = Word32

    const int G_FILE_CHUNK_SIZE = 1048576; // 1MB
    const int G_MAX_FILE_SIZE = 209715200; // Max size: 200MB

    const int G_BEACON_PREFIX_LEN = 4;
    const int G_BEACON_ID_LEN = 0;
    const int G_BEACON_NONCE_LEN = 14;
    const int G_BEACON_MSG_LEN = 18; // = prefix_len + id_len + nonce_len
    const int G_BEACON_MSG_HISTORY = 10;
    const int G_BEACON_REF_INT = 20; // in seconds

    const int G_BEACON_PROXIMITY_TIMEOUT_LIM = 5; // how many heartbeat messages can a device miss out before deemed outside
    const bool G_BEACON_PROXIMITY_ENABLE = false;

    const int SOCKET_SEND_FLAGS = 0;
    const int READ_BUFFER_SIZE = G_DATA_BUF_SIZE;

    extern std::string default_device_ip;
    extern unsigned short default_device_port;

    extern std::string default_admin_ip;
    extern unsigned short default_admin_port;

    extern std::string accessor_ip;
    extern unsigned short accessor_port;

    extern std::string default_storage_ip;
    extern unsigned short default_storage_port;
    extern std::string storage_content_location;
    extern std::string storage_owner_location;
}

#endif //TEO_TEO_PARAMS_HPP
