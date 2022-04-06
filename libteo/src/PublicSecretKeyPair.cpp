//
// Created by han on 2/18/21.
//

#include <stdexcept>
#include <cassert>
#include <cstring>
#include <iostream>
#include "libtot/PublicSecretKeyPair.hpp"


namespace libtot {
    int PublicSecretKeyPair::deterministic_seed_counter = 0;

    void PublicSecretKeyPair::initialize() {
        allocate_keypair();
    }

    void PublicSecretKeyPair::initialize(const unsigned char *seed) {
        allocate_keypair();
        generate_seed_keypair(seed);
    }

    void PublicSecretKeyPair::initialize(const unsigned char *pk_in, size_t pk_in_len,
                                         const unsigned char *sk_in, size_t sk_in_len) {
        allocate_keypair();
        set_pk(pk_in, pk_in_len);
        set_sk(sk_in, sk_in_len);
    }


    void PublicSecretKeyPair::initialize(const unsigned char *pk_in, size_t pk_in_len) {
        allocate_keypair();
        set_pk(pk_in, pk_in_len);
    }

    void PublicSecretKeyPair::initialize(const std::string &pk_in) {
        allocate_keypair();
        set_pk(reinterpret_cast<const unsigned char*>(pk_in.c_str()), pk_in.size());
    }

    PublicSecretKeyPair::~PublicSecretKeyPair() {
        release();
    }

    int PublicSecretKeyPair::generate_seed_keypair(const unsigned char *seed) {
        bool clean_seed = false;
        if (seed == nullptr) {
            seed = new unsigned char[get_seed_size()]{0};
            clean_seed = true;
//#ifndef NDEBUG
//            // Increment seed one-by-one
//            memcpy((void *) seed, &deterministic_seed_counter, sizeof(deterministic_seed_counter));
//            deterministic_seed_counter++;
//#else
            // Use random seed in release mode.
            randombytes_buf((void *) seed, crypto_box_SEEDBYTES);
//#endif
        }
        int res = crypto_seed_keypair_generator(pk, sk, seed);
        if (res != 0) {
            throw std::runtime_error("Error: generating Client keypair.");
        }
        valid_pk = true;
        valid_sk = true;
        if (clean_seed) {
            delete[] seed;
        }
        return res;
    }

    void PublicSecretKeyPair::set_pk(const unsigned char *pk_in, size_t pk_in_len) {
        assert(pk_in_len >= get_pk_size());
        memcpy(pk, pk_in, get_pk_size());
        valid_pk = true;
    }

    void PublicSecretKeyPair::set_sk(const unsigned char *sk_in, size_t sk_in_len) {
        assert(sk_in_len >= get_sk_size());
        memcpy(sk, sk_in, get_sk_size());
        valid_sk = true;
    }

    const unsigned char *PublicSecretKeyPair::get_pk() const {
        if (has_valid_pk()) {
            return pk;
        } else {
            return nullptr;
        }
    }

    const unsigned char *PublicSecretKeyPair::get_sk() const {
        if (has_valid_sk()) {
            return sk;
        } else {
            return nullptr;
        }
    }

    void PublicSecretKeyPair::reset_keypair() {
        assert(pk != nullptr && sk != nullptr);
        memset(pk, 0, get_pk_size());
        memset(sk, 0, get_sk_size());
        valid_pk = false;
        valid_sk = false;
    }

    void PublicSecretKeyPair::allocate_keypair() {
        assert(pk == nullptr && sk == nullptr);
        pk = new uint8_t[get_pk_size()]{0};
        sk = new uint8_t[get_sk_size()]{0};
        valid_pk = false;
        valid_sk = false;
        initialized = true;
    }

    bool PublicSecretKeyPair::has_valid_pk() const {
        return pk != nullptr && valid_pk;
    }

    bool PublicSecretKeyPair::has_valid_sk() const {
        return sk != nullptr && valid_sk;
    }

    bool PublicSecretKeyPair::is_initialized() const {
        return initialized;
    }

    void PublicSecretKeyPair::release() {
        initialized = false;
        delete[] pk;
        pk = nullptr;
        delete[] sk;
        sk = nullptr;

        valid_pk = false;
        valid_sk = false;
    }


}