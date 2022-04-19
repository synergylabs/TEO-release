//
// Created by han on 2/22/21.
//

#include "teo/PKAEADKey.hpp"

namespace teo {
    PKAEADKey::PKAEADKey(const unsigned char *seed) {
        initialize(seed);
    }

    PKAEADKey::PKAEADKey(const uint8_t *pk_in, size_t pk_in_len) {
        initialize(pk_in, pk_in_len);
    }

    size_t PKAEADKey::get_pk_size() const {
        return PUBLICKEYBYTES;
    }

    size_t PKAEADKey::get_sk_size() const {
        return SECRETKEYBYTES;
    }

    size_t PKAEADKey::get_seed_size() const {
        return SEEDBYTES;
    }

    int PKAEADKey::crypto_seed_keypair_generator(unsigned char *pk, unsigned char *sk, const unsigned char *seed) {
        return crypto_box_seed_keypair(pk, sk, seed);
    }

    int PKAEADKey::box_seal_open(unsigned char *m, const unsigned char *c, unsigned long long int clen) const {
        return crypto_box_seal_open(m, c, clen, get_pk(), get_sk());
    }

    PKAEADKey::PKAEADKey() {
        initialize();
    }

    PKAEADKey::PKAEADKey(const PKAEADKey &obj) {
        release();
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


    PKAEADKey::PKAEADKey(const uint8_t *pk_in, size_t pk_in_len, const uint8_t *sk_in,
                         size_t sk_in_len) {
        initialize(pk_in, pk_in_len, sk_in, sk_in_len);
    }

    PKAEADKey &PKAEADKey::operator=(const PKAEADKey &obj) {
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

    int PKAEADKey::box_easy(unsigned char *c, const unsigned char *m, unsigned long long int mlen,
                            const unsigned char *n, const unsigned char *recv_pk) {
        return crypto_box_easy(c, m, mlen, n, recv_pk, get_sk());
    }

    int PKAEADKey::box_open_easy(unsigned char *m, const unsigned char *c, unsigned long long int clen,
                                 const unsigned char *n, const unsigned char *sender_pk) {
        return crypto_box_open_easy(m, c, clen, n, sender_pk, get_sk());
    }


}