//
// Created by han on 2/22/21.
//

#ifndef TEO_PKAEADKEY_HPP
#define TEO_PKAEADKEY_HPP

#include "PublicSecretKeyPair.hpp"

namespace teo {
    class PKAEADKey : public PublicSecretKeyPair {
    public:
        static const int PUBLICKEYBYTES = crypto_box_PUBLICKEYBYTES;
        static const int SECRETKEYBYTES = crypto_box_SECRETKEYBYTES;
        static const int SEEDBYTES = crypto_box_SEEDBYTES;
        static const int SEALBYTES = crypto_box_SEALBYTES;
        static const int MACBYTES = crypto_box_MACBYTES;

        PKAEADKey();

        explicit PKAEADKey(const unsigned char *seed);

        PKAEADKey(const uint8_t *pk_in, size_t pk_in_len);

        PKAEADKey(const uint8_t *pk_in, size_t pk_in_len,
                  const uint8_t *sk_in, size_t sk_in_len);

        // Overload copy constructor
        PKAEADKey(const PKAEADKey &obj);

        // Overload assignment operator
        PKAEADKey &operator=(const PKAEADKey &obj);

        size_t get_pk_size() const override;

        size_t get_sk_size() const override;

        size_t get_seed_size() const override;

        int crypto_seed_keypair_generator(unsigned char *pk,
                                          unsigned char *sk,
                                          const unsigned char *seed) override;

        int box_seal_open(unsigned char *m, const unsigned char *c,
                          unsigned long long clen) const;

        int box_easy(unsigned char *c, const unsigned char *m, unsigned long long mlen, const unsigned char *n,
                     const unsigned char *recv_pk);

        int box_open_easy(unsigned char *m, const unsigned char *c, unsigned long long clen,
                          const unsigned char *n, const unsigned char *sender_pk);
    };
}

#endif //TEO_PKAEADKEY_HPP
