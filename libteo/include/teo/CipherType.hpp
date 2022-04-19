//
// Created by Han Zhang on 3/16/21.
//

#ifndef TEO_CIPHERTYPE_HPP
#define TEO_CIPHERTYPE_HPP

#include "AsymmetricEncryptionKeySet.hpp"
#include "teo_params.hpp"
#include "teo_helper.hpp"
#include "SharedSecretKey.hpp"
#include "Sieve.hpp"

namespace teo
{
    enum CipherType : uint64_t
    {
        no_type,

        initialization_request,
        initialization_device_info,
        initialization_admin_reply,

        acquire_pre_auth_token,

        claim_device_request,
        claim_device_response,

        data_store_sieve_cred_request,
        data_store_sieve_cred_response,
        data_store_upload_notification,

        data_access_fetch,
        data_access_response,

        data_reencryption_pre_request,
        data_reencryption_pre_response,
        data_reencryption_request
    };

    struct CiphertextInitializationRequest
    {
        CipherType type = CipherType::no_type;
        uint8_t admin_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
        uint8_t admin_challenge[G_CHALLENGE_SIZE]{};
    };

    struct CiphertextInitializationDeviceInfo
    {
        CipherType type = CipherType::no_type;
        uint8_t admin_challenge[G_CHALLENGE_SIZE]{};
        uint8_t device_challenge[G_CHALLENGE_SIZE]{};
        uint8_t device_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{};
    };

    struct CiphertextInitializationAdminReply
    {
        CipherType type = CipherType::no_type;
        uint8_t device_challenge[G_CHALLENGE_SIZE]{};
        uint8_t device_proof[AsymmetricEncryptionKeySet::SIGNATURE_SIZE]{};
    };

    struct CiphertextAcquirePreAuthTokenResponse
    {
        CipherType type = CipherType::no_type;
        uint8_t token[AsymmetricEncryptionKeySet::SIGNATURE_SIZE]{};
    };

    struct CiphertextClaimDeviceRequest
    {
        CipherType type = CipherType::no_type;
        uint8_t token[AsymmetricEncryptionKeySet::SIGNATURE_SIZE]{};
        uint8_t user_challenge[G_CHALLENGE_SIZE]{};
    };

    struct CiphertextClaimDeviceResponse
    {
        CipherType type = CipherType::no_type;
        uint8_t challenge_decrypted[G_CHALLENGE_SIZE]{};
    };

    struct CiphertextDataStoreSieveCredRequest
    {
        CipherType type = CipherType::no_type;
        uint8_t session_key[SharedSecretKey::KEY_SIZE]{};
        uint8_t session_id[G_SESS_ID_SIZE]{};
    };

    struct CiphertextDataStoreSieveCredResponse
    {
        CipherType type = CipherType::no_type;
        uint8_t sieve_key[SIEVE_KEY_SIZE]{};
        uint8_t sieve_nonce[SIEVE_NONCE_SIZE]{};
    };

    struct CiphertextDataStoreUploadNotification
    {
        CipherType type = CipherType::no_type;
        uint8_t session_id[G_SESS_ID_SIZE]{};
        int8_t sieve_data_block_uuid[UUID::UUID_SIZE]{};
        int8_t encrypted_metadata_uuid[UUID::UUID_SIZE]{};
    };

    struct CiphertextDataAccessFetch
    {
        CipherType type = CipherType::no_type;
        int8_t sieve_data_block_uuid[UUID::UUID_SIZE]{};
        uint8_t random_challenge[G_CHALLENGE_SIZE]{};
    };

    struct CiphertextDataAccessResponse
    {
        CipherType type = CipherType::no_type;
        uint8_t sieve_key[SIEVE_KEY_SIZE]{};
        uint8_t random_challenge_response[G_CHALLENGE_SIZE]{};
    };

    struct MetadataBlock
    {
        uint8_t data_key[G_SECRET_SHARE_CHANNEL_ID_LEN + SharedSecretKey::KEY_SIZE]{};
    };

    struct CiphertextDataReencryptionPreRequest
    {
        CipherType type = CipherType::no_type;
        int8_t sieve_data_block_uuid[UUID::UUID_SIZE]{};
        int8_t encrypted_metadata_uuid[UUID::UUID_SIZE]{};
        uint8_t user_nonce[G_CHALLENGE_SIZE]{};
    };

    struct CiphertextDataReencryptionPreResponse
    {
        CipherType type = CipherType::no_type;
        uint8_t user_nonce[G_CHALLENGE_SIZE]{};
        uint8_t storage_nonce[G_CHALLENGE_SIZE]{};
    };

    struct CiphertextDataReencryptionRequest
    {
        CipherType type = CipherType::no_type;
        RekeyToken rekey_token;
        uint8_t noti_token[G_CHALLENGE_SIZE]{};
        uint8_t user_nonce[G_CHALLENGE_SIZE]{};
        uint8_t storage_nonce[G_CHALLENGE_SIZE]{};
    };
}

#endif //TEO_CIPHERTYPE_HPP
