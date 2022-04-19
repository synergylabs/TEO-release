//
// Created by Han Zhang on 3/22/21.
//

#ifndef TEO_SIEVE_HPP
#define TEO_SIEVE_HPP

#include <decaf.hxx>
#include <decaf/secure_buffer.hxx>
#include <decaf/shake.hxx>
#include <decaf/spongerng.hxx>

#include "teo_params.hpp"

namespace teo
{
    /*
    * Sieve cryptography functions
    */

    typedef typename TEO_EC_Group::Scalar RekeyToken;

    const int SIEVE_KEY_SIZE = TEO_EC_Group::Scalar::SER_BYTES;
    const int SIEVE_NONCE_SIZE = 32;

    class SieveKey
    {
    private:
        TEO_EC_Group::Scalar key;
        uint8_t nonce[SIEVE_NONCE_SIZE]{};

        void clone(const SieveKey &obj);

    public:
        static const int CHUNK_SIZE = TEO_EC_Group::Point::HASH_BYTES;

        SieveKey();

        SieveKey(const uint8_t *key_buf, size_t key_buf_len);

        SieveKey(const uint8_t *key_buf, size_t key_buf_len,
                 const uint8_t *nonce_buf, size_t nonce_buf_len);

        // Overload copy constructor
        SieveKey(const SieveKey &obj);

        // Overload assignment operator
        SieveKey &operator=(const SieveKey &obj);

        int set_nonce(const uint8_t *nonce_buf, size_t nonce_buf_len);

        const uint8_t *get_nonce();

        int serialize_key_into(unsigned char *buf, size_t buf_len);

        int serialize_nonce_into(unsigned char *buf, size_t buf_len);

        int encrypt(const uint8_t *msg, size_t msg_len,
                    std::vector<int> &hints, decaf::SecureBuffer &ciphertext);

        int decrypt(const uint8_t *cipher, size_t cipher_len,
                    uint8_t *msg, const std::vector<int> &hints);

        /**
         * Generating pseudorandom function from Sieve paper.
         * @return f(k, x), where k == key, x == nonce + offset
         */
        static TEO_EC_Group::Point sieve_f_k_x(TEO_EC_Group::Scalar k,
                                                  unsigned int offset,
                                                  const uint8_t *nonce);

        RekeyToken gen_rekey_token(SieveKey &new_key);

        static int apply_rekey_token_cipher(uint8_t *cipher, size_t cipher_len,
                                     const std::vector<int> &hints, RekeyToken token, const uint8_t *nonce);

        int apply_rekey_token_key(RekeyToken token);
    };

    /**
     * Generate an initial secret key k from random.
     * @return k
     */
    TEO_EC_Group::Scalar sieve_keygen(
        decaf::SpongeRng::Deterministic det = decaf::SpongeRng::RANDOM);

    /**
     * Generating pseudorandom function from Sieve paper.
     * @param k The secret key.
     * @param x The source value.
     * @return f(k, x).
     */
    decaf::SecureBuffer test_sieve_prf(uint32_t k, const char *x);

    decaf::SecureBuffer test_sieve_prf(const TEO_EC_Group::Scalar &k, const char *x);

    decaf::SecureBuffer test_sieve_encrypt_block(
        const std::string &message,
        const TEO_EC_Group::Point &fp);

}

#endif //TEO_SIEVE_HPP
