//
// Created by Han Zhang on 3/17/21.
//

#ifndef LIBTOT_SIGNATURETYPE_HPP
#define LIBTOT_SIGNATURETYPE_HPP

namespace libtot
{
    enum SignatureType : uint32_t
    {
        admin_sign_device_proof,
        admin_sign_pre_auth_token,
        user_sign_rekey_token
    };

    struct SignatureDeviceProofContent
    {
        SignatureType type = SignatureType::admin_sign_device_proof;
        uint8_t device_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{0};
    };

    struct SignaturePreAuthToken
    {
        SignatureType type = SignatureType::admin_sign_pre_auth_token;
        uint8_t user_pubkey[AsymmetricEncryptionKeySet::FULL_PK_SIZE]{0};
    };

    struct SignatureRekeyToken
    {
        SignatureType type = SignatureType::user_sign_rekey_token;
        int8_t sieve_uuid[UUID::UUID_SIZE]{};
        RekeyToken rekey_token;
    };
}

#endif //LIBTOT_SIGNATURETYPE_HPP
