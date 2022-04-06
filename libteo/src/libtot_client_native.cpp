#include <cstring>

#include "libtot/CipherType.hpp"
#include "libtot/SignatureType.hpp"
#include "libtot/libtot_client_native.hpp"
#include "libtot/libtot_logger.hpp"
#include "libtot/libtot_network.hpp"

namespace libtot
{
    int admin_initialize_device_impl(const char *device_ip_load, const int device_port_in,
                                     const void *user_pubkey_ptr, size_t user_pubkey_len,
                                     SharedSecretKey &setup_token, AsymmetricEncryptionKeySet &keySet)
    {

        int sockfd = network_connect(device_ip_load, device_port_in);
        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);

        // send Initialization Request
        CiphertextInitializationRequest request_payload;
        request_payload.type = CipherType::initialization_request;
        if (user_pubkey_ptr != nullptr)
        {
            memcpy(request_payload.admin_pubkey, user_pubkey_ptr, user_pubkey_len);
        }
        else
        {
            keySet.get_full_pk(request_payload.admin_pubkey, user_pubkey_len);
        }
        random_buf(request_payload.admin_challenge, G_CHALLENGE_SIZE);

        size_t request_ciphertext_len = SharedSecretKey::get_cipher_len(sizeof(request_payload));
        auto request_ciphertext = new uint8_t[request_ciphertext_len];
        setup_token.encrypt(request_ciphertext, request_ciphertext_len,
                            reinterpret_cast<const uint8_t *>(&request_payload),
                            sizeof(request_payload));

        auto setup_header_obj = builder.CreateVector(setup_token.get_header(),
                                                     SharedSecretKey::HEADER_SIZE);
        auto request_ciphertext_obj = builder.CreateVector(request_ciphertext,
                                                           request_ciphertext_len);
        auto request_msg = CreateInitializationRequest(builder, setup_header_obj,
                                                       request_ciphertext_obj);
        builder.Finish(request_msg);

        network_send_message_type(sockfd, MessageType_INITIALIZATION_REQUEST);
        network_send(sockfd, builder.GetBufferPointer(), builder.GetSize());

        // receive Initialization Device Info
        if (network_read_message_type(sockfd) != MessageType_INITIALIZATION_DEVICE_INFO)
        {
            LOGW("Unexpected device response during initialization");
            delete[] request_ciphertext;
            return -1;
        }

        uint8_t device_info_buf[READ_BUFFER_SIZE]{0};
        network_read(sockfd, device_info_buf, READ_BUFFER_SIZE);
        auto device_info_msg = GetInitializationDeviceInfo(device_info_buf);

        CiphertextInitializationDeviceInfo device_info;

        auto ciphertext_data = device_info_msg->ciphertext()->Data();
        auto ciphertext_data_len = device_info_msg->ciphertext()->size();

        keySet.box_seal_open(reinterpret_cast<uint8_t *>(&device_info),
                             sizeof(device_info),
                             ciphertext_data,
                             ciphertext_data_len);

        if (device_info.type != CipherType::initialization_device_info)
        {
            LOGW("Incorrect device info type");
            delete[] request_ciphertext;
            return -2;
        }
        if (memcmp(device_info.admin_challenge, request_payload.admin_challenge, G_CHALLENGE_SIZE) !=
            0)
        {
            LOGW("Incorrect initialization device info message!");
            delete[] request_ciphertext;
            return -3;
        }

        LOGV("Event: Initialization admin acquire device");

        // Create device proof
        SignatureDeviceProofContent device_proof_content;
        memcpy(device_proof_content.device_pubkey,
               device_info.device_pubkey, AsymmetricEncryptionKeySet::FULL_PK_SIZE);

        // Send Initialization Admin Reply
        CiphertextInitializationAdminReply reply_payload;
        reply_payload.type = CipherType::initialization_admin_reply;
        memcpy(reply_payload.device_challenge,
               device_info.device_challenge, G_CHALLENGE_SIZE);
        keySet.sign_detached(reply_payload.device_proof,
                             reinterpret_cast<const unsigned char *>(&device_proof_content),
                             sizeof(device_proof_content));
        uint8_t msg_nonce[AsymmetricEncryptionKeySet::NONCE_SIZE]{0};
        size_t admin_reply_ciphertext_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(
            sizeof(reply_payload));
        auto admin_reply_ciphertext = new uint8_t[admin_reply_ciphertext_len];
        keySet.box_easy(admin_reply_ciphertext, admin_reply_ciphertext_len,
                        reinterpret_cast<const uint8_t *>(&reply_payload),
                        sizeof(reply_payload), msg_nonce, device_info.device_pubkey);

        builder.Clear();
        auto admin_reply_msg_nonce_obj = builder.CreateVector(msg_nonce,
                                                              AsymmetricEncryptionKeySet::NONCE_SIZE);
        auto admin_reply_ciphertext_obj = builder.CreateVector(admin_reply_ciphertext,
                                                               admin_reply_ciphertext_len);
        auto admin_reply_msg = CreateInitializationAdminReply(builder, admin_reply_msg_nonce_obj,
                                                              admin_reply_ciphertext_obj);
        builder.Finish(admin_reply_msg);

        network_send_message_type(sockfd, MessageType_INITIALIZATION_ADMIN_REPLY);
        network_send(sockfd, builder.GetBufferPointer(), builder.GetSize());

        delete[] request_ciphertext;
        delete[] admin_reply_ciphertext;

        return 0;
    }

    int user_acquire_pre_auth_token_impl(const char *admin_ip_load, const int admin_port_in, const uint8_t *admin_pubkey,
                                         AsymmetricEncryptionKeySet &keySet, PreAuthToken &pre_auth_token)
    {
        int sockfd = network_connect(admin_ip_load, admin_port_in);

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);

        uint8_t pubkey_buf[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{0};
        keySet.get_full_pk(pubkey_buf, sizeof(pubkey_buf));

        auto user_pubkey_obj = builder.CreateVector(pubkey_buf, sizeof(pubkey_buf));
        auto request_msg = CreateAcquirePreAuthTokenRequest(builder, user_pubkey_obj);
        builder.Finish(request_msg);

        network_send_message_type(sockfd, MessageType_ACQUIRE_PRE_AUTH_TOKEN_REQUEST);
        network_send(sockfd, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(sockfd) != MessageType_ACQUIRE_PRE_AUTH_TOKEN_RESPONSE)
        {
            LOGW("Unexpected response type for pre auth token");
            return -1;
        }

        uint8_t response_buf[READ_BUFFER_SIZE]{0};
        network_read(sockfd, response_buf, READ_BUFFER_SIZE);
        auto response_msg = GetAcquirePreAuthTokenResponse(response_buf);

        CiphertextAcquirePreAuthTokenResponse response_payload;
        keySet.box_open_easy(reinterpret_cast<uint8_t *>(&response_payload), sizeof(response_payload),
                             response_msg->ciphertext()->data(), response_msg->ciphertext()->size(),
                             response_msg->box_nonce()->data(), admin_pubkey);

        if (response_payload.type != CipherType::acquire_pre_auth_token)
        {
            LOGW("Incorrect response type");
            return -1;
        }

        pre_auth_token = PreAuthToken(response_payload.token, sizeof(response_payload.token));

        LOGV("Event: Acquire pre auth token user receive\n");

        return 0;
    }

    int user_claim_device_impl(AsymmetricEncryptionKeySet &userKeySet, PreAuthToken &pre_auth_token,
                               const char *device_ip_load, const int device_port_in, const uint8_t *admin_pubkey,
                               bool exclusive, uint8_t *claimed_device, size_t claimed_device_len)
    {
        if (!pre_auth_token.is_initialized())
        {
            LOGW("Uninitialized pre auth token!");
            return -1;
        }

        int sockfd = network_connect(device_ip_load, device_port_in);

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);

        uint8_t user_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
        userKeySet.get_full_pk(user_pubkey, sizeof(user_pubkey));
        auto user_pubkey_obj = builder.CreateVector(user_pubkey, sizeof(user_pubkey));
        auto discovery_msg = CreateClaimDeviceDiscovery(builder, user_pubkey_obj);
        builder.Finish(discovery_msg);

        network_send_message_type(sockfd, MessageType_CLAIM_DEVICE_DISCOVERY);
        network_send(sockfd, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(sockfd) != MessageType_CLAIM_DEVICE_DISCOVERY_RESPONSE)
        {
            LOGW("Unexpected message type");
            return -1;
        }

        uint8_t discovery_response_buf[READ_BUFFER_SIZE];
        network_read(sockfd, discovery_response_buf, sizeof(discovery_response_buf));
        auto discovery_response_msg = GetClaimDeviceDiscoveryResponse(discovery_response_buf);

        // Verify valid device proof
        SignatureDeviceProofContent expected_proof{};
        memcpy(expected_proof.device_pubkey, discovery_response_msg->device_pubkey()->Data(),
               discovery_response_msg->device_pubkey()->size());

        if (AsymmetricEncryptionKeySet::sign_verify_detached(discovery_response_msg->valid_device_proof()->Data(),
                                                             discovery_response_msg->valid_device_proof()->size(),
                                                             reinterpret_cast<const uint8_t *>(&expected_proof),
                                                             sizeof(expected_proof),
                                                             admin_pubkey) != 0)
        {
            LOGW("Invalid device proof");
            return -1;
        }

        CiphertextClaimDeviceRequest request_payload;
        request_payload.type = CipherType::claim_device_request;
        memcpy(request_payload.token, pre_auth_token.get_token(), PreAuthToken::get_token_len());
        random_buf(request_payload.user_challenge, sizeof(request_payload.user_challenge));

        size_t request_cipher_buf_len = AsymmetricEncryptionKeySet::get_box_easy_cipher_len(
            sizeof(request_payload));
        auto request_cipher_buf = new uint8_t[request_cipher_buf_len]{};
        uint8_t request_nonce[AsymmetricEncryptionKeySet::NONCE_SIZE]{};
        userKeySet.box_easy(request_cipher_buf, request_cipher_buf_len,
                            reinterpret_cast<const uint8_t *>(&request_payload), sizeof(request_payload),
                            request_nonce, discovery_response_msg->device_pubkey()->Data());

        builder.Clear();
        auto nonce_obj = builder.CreateVector(request_nonce, sizeof(request_nonce));
        auto ciphertext_obj = builder.CreateVector(request_cipher_buf, request_cipher_buf_len);
        int group_mode = exclusive ? 0 : 1;
        auto request_msg = CreateClaimDeviceRequest(builder, nonce_obj, ciphertext_obj, group_mode);
        builder.Finish(request_msg);

        network_send_message_type(sockfd, MessageType_CLAIM_DEVICE_REQUEST);
        network_send(sockfd, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(sockfd) != MessageType_CLAIM_DEVICE_RESPONSE)
        {
            LOGW("Unexpected message for claim device response");
            delete[] request_cipher_buf;
            return -1;
        }

        uint8_t response_buf[READ_BUFFER_SIZE];
        network_read(sockfd, response_buf, sizeof(response_buf));
        auto response_msg = GetClaimDeviceResponse(response_buf);

        if (!response_msg->status())
        {
            LOGW("Device response is failure");
            delete[] request_cipher_buf;
            return -1;
        }

        CiphertextClaimDeviceResponse response_payload;
        userKeySet.box_open_easy(reinterpret_cast<uint8_t *>(&response_payload),
                                 sizeof(response_payload),
                                 response_msg->challenge_response_encrypted()->Data(),
                                 response_msg->challenge_response_encrypted()->size(),
                                 response_msg->response_nonce()->Data(),
                                 discovery_response_msg->device_pubkey()->Data());

        if (response_payload.type != CipherType::clain_device_response)
        {
            LOGW("Invalid response type");
            delete[] request_cipher_buf;
            return -1;
        }

        if (memcmp(response_payload.challenge_decrypted, request_payload.user_challenge,
                   sizeof(request_payload.user_challenge)) != 0)
        {
            LOGW("Mismatch challenge response");
            delete[] request_cipher_buf;
            return -1;
        }

        memcpy(claimed_device, discovery_response_msg->device_pubkey()->Data(), claimed_device_len);

        LOGV("Event: Claim device user finish device");

        delete[] request_cipher_buf;

        return 0;
    }

    int client_register_ip_kms_impl(const uint8_t *client_pubkey, size_t client_pubkey_len,
                                    const char *client_ip_load, const int client_port_in,
                                    const char *storage_ip_load, const int storage_port_in)
    {
        int conn = network_connect(storage_ip_load, storage_port_in);

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto pubkey_obj = builder.CreateVector(client_pubkey, client_pubkey_len);
        auto ip_obj = builder.CreateString(client_ip_load);
    auto register_msg = CreateUtilRegisterIp(builder, pubkey_obj, ip_obj, client_port_in);
        builder.Finish(register_msg);

        network_send_message_type(conn, MessageType_UTIL_REGISTER_IP);
        network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        return 0;
    }

    int client_fetch_ip_kms_impl(const uint8_t *pk, size_t pk_len,
                                 const char *storage_ip_load, const int storage_port_in,
                                 std::string &res_ip, int &res_port)
    {
        assert(pk_len >= AsymmetricEncryptionKeySet::FULL_PK_SIZE);

        int conn = network_connect(storage_ip_load, storage_port_in);

        flatbuffers::FlatBufferBuilder builder(G_FBS_SIZE);
        auto pubkey_obj = builder.CreateVector(pk, pk_len);
        auto req_msg = CreateUtilFetchIpRequest(builder, pubkey_obj);
        builder.Finish(req_msg);

        network_send_message_type(conn, MessageType_UTIL_FETCH_IP_REQUEST);
        network_send(conn, builder.GetBufferPointer(), builder.GetSize());

        if (network_read_message_type(conn) != MessageType_UTIL_FETCH_IP_RESPONSE)
        {
            LOGW("Wrong response type from KMS");
            return -1;
        }

        uint8_t buf[G_DATA_BUF_SIZE]{};
        network_read(conn, buf, sizeof(buf));
        auto res_msg = GetUtilFetchIpResponse(buf);

        res_ip = std::string(res_msg->ip()->data());
        res_port = res_msg->port();

        return 0;
    }
}