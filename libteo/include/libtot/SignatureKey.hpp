//
// Created by han on 2/22/21.
//

#ifndef LIBTOT_SIGNATUREKEY_HPP
#define LIBTOT_SIGNATUREKEY_HPP

#include "PublicSecretKeyPair.hpp"

namespace libtot {
    class SignatureKey : public PublicSecretKeyPair {
    public:
        static const int PUBLICKEYBYTES = crypto_sign_PUBLICKEYBYTES;
        static const int SECRETKEYBYTES = crypto_sign_SECRETKEYBYTES;
        static const int SEEDBYTES = crypto_sign_SEEDBYTES;
        static const int SIGNBYTES = crypto_sign_BYTES;

        SignatureKey();

        explicit SignatureKey(const unsigned char *seed);

        SignatureKey(const uint8_t *pk_in, size_t pk_in_len);

        SignatureKey(const uint8_t *pk_in, size_t pk_in_len,
                     const uint8_t *sk_in, size_t sk_in_len);

        // Overload copy constructor
        SignatureKey(const SignatureKey &obj);

        // Overload assignment operator
        SignatureKey &operator=(const SignatureKey &obj);

        size_t get_pk_size() const override;

        size_t get_sk_size() const override;

        size_t get_seed_size() const override;

        int crypto_seed_keypair_generator(unsigned char *pk,
                                          unsigned char *sk,
                                          const unsigned char *seed) override;

        int sign_detached(unsigned char *sig, const unsigned char *m, unsigned long long mlen);
    };
}

#endif //LIBTOT_SIGNATUREKEY_HPP
