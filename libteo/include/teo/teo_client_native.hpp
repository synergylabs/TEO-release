// teo's Client interface to faciliate C/C++ native integration with mobile client
#ifndef TEO_TEO_CLIENT_NATIVE_H
#define TEO_TEO_CLIENT_NATIVE_H

#include "PreAuthToken.hpp"

namespace teo
{
    int admin_initialize_device_impl(const char *device_ip_load, const int device_port_in,
                                     const void *user_pubkey_ptr, size_t user_pubkey_len,
                                     SharedSecretKey &setup_token,
                                     AsymmetricEncryptionKeySet &keySet);

    int user_acquire_pre_auth_token_impl(const char *admin_ip_load, const int admin_port_in,
                                         const uint8_t *admin_pubkey,
                                         AsymmetricEncryptionKeySet &userKeySet,
                                         PreAuthToken &pre_auth_token);

    int user_claim_device_impl(AsymmetricEncryptionKeySet &userKeySet, PreAuthToken &pre_auth_token,
                               const char *device_ip_load, const int device_port_in, const uint8_t *admin_pubkey,
                               bool exclusive, uint8_t *claimed_device, size_t claimed_device_len);

    int client_register_ip_kms_impl(const uint8_t *client_pubkey, size_t client_pubkey_len,
                                    const char *client_ip_load, const int client_port_in,
                                    const char *storage_ip_load, const int storage_port_in);

    int client_fetch_ip_kms_impl(const uint8_t *query_pubkey, size_t query_pubkey_len,
                                 const char *storage_ip_load, const int storage_port_in,
                                 std::string &res_ip, int &res_port);
}

#endif // TEO_TEO_CLIENT_NATIVE_H
