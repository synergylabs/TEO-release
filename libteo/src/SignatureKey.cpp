//
// Created by han on 2/22/21.
//

#include "teo/SignatureKey.hpp"

namespace teo {

    size_t SignatureKey::get_pk_size() const {
        return PUBLICKEYBYTES;
    }

    size_t SignatureKey::get_sk_size() const {
        return SECRETKEYBYTES;
    }

    size_t SignatureKey::get_seed_size() const {
        return SEEDBYTES;
    }

    int SignatureKey::crypto_seed_keypair_generator(unsigned char *pk, unsigned char *sk, const unsigned char *seed) {
        return crypto_sign_seed_keypair(pk, sk, seed);
    }

    SignatureKey::SignatureKey(const uint8_t *pk_in, size_t pk_in_len) {
        initialize(pk_in, pk_in_len);
    }

    SignatureKey::SignatureKey() {
        initialize();
    }

    SignatureKey::SignatureKey(const unsigned char *seed) {
        initialize(seed);
    }

    SignatureKey::SignatureKey(const SignatureKey &obj) {
        if (!obj.is_initialized()) {
            return;
        }
        initialize();
        if (obj.has_valid_pk()) {
            set_pk(obj.get_pk(), obj.get_pk_size());
        }
        if (obj.has_valid_sk()) {
            set_sk(obj.get_sk(), obj.get_sk_size());
        }
    }

    SignatureKey &SignatureKey::operator=(const SignatureKey &obj) {
        if (this == &obj) {
            return *this;
        }

        release();
        if (!obj.is_initialized()) {
            return *this;
        }
        initialize();
        if (obj.has_valid_pk()) {
            set_pk(obj.get_pk(), obj.get_pk_size());
        }
        if (obj.has_valid_sk()) {
            set_sk(obj.get_sk(), obj.get_sk_size());
        }
        return *this;
    }

    int SignatureKey::sign_detached(unsigned char *sig, const unsigned char *m, unsigned long long int mlen) {
        return crypto_sign_detached(sig, nullptr, m, mlen, get_sk());
    }

    SignatureKey::SignatureKey(const uint8_t *pk_in, size_t pk_in_len, const uint8_t *sk_in,
                               size_t sk_in_len) {
        initialize(pk_in, pk_in_len, sk_in, sk_in_len);
    }


}