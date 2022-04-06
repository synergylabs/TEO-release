//
// Created by han on 2/22/21.
//

#ifndef LIBTOT_ASYMMETRICENCRYPTIONKEYSET_HPP
#define LIBTOT_ASYMMETRICENCRYPTIONKEYSET_HPP

#include <string>

#include "PKAEADKey.hpp"
#include "SignatureKey.hpp"

namespace libtot
{
    class AsymmetricEncryptionKeySet
    {
    private:
        static const int PK_BUF_AEAD_OFFSET = 0;
        static const int PK_BUF_SIGN_OFFSET = PK_BUF_AEAD_OFFSET + PKAEADKey::PUBLICKEYBYTES;

        static const int SK_BUF_AEAD_OFFSET = 0;
        static const int SK_BUF_SIGN_OFFSET = SK_BUF_AEAD_OFFSET + PKAEADKey::SECRETKEYBYTES;

        PKAEADKey aead_key;
        SignatureKey sign_key;

    public:
        static const size_t FULL_PK_SIZE = PKAEADKey::PUBLICKEYBYTES + SignatureKey::PUBLICKEYBYTES;
        static const size_t FULL_SK_SIZE = PKAEADKey::SECRETKEYBYTES + SignatureKey::SECRETKEYBYTES;
        static const size_t SIGNATURE_SIZE = SignatureKey::SIGNBYTES;
        static const size_t NONCE_SIZE = crypto_box_NONCEBYTES;

        AsymmetricEncryptionKeySet();

        explicit AsymmetricEncryptionKeySet(const uint8_t *pub_key);

        AsymmetricEncryptionKeySet(const uint8_t *pub_key_buf, size_t pub_key_buf_len);

        AsymmetricEncryptionKeySet(const uint8_t *pub_key_buf, size_t pub_key_buf_len,
                                   const uint8_t *priv_key_buf, size_t priv_key_buf_len);

        // Overload copy constructor
        AsymmetricEncryptionKeySet(const AsymmetricEncryptionKeySet &obj);

        // Overload assignment operator
        AsymmetricEncryptionKeySet &operator=(const AsymmetricEncryptionKeySet &obj);

        const PKAEADKey &get_aead_key();

        const SignatureKey &get_sign_key();

        int get_full_pk(uint8_t *buf, size_t buf_len) const;

        std::string get_full_pk() const;

        int get_full_sk(uint8_t *buf, size_t buf_len) const;

        void print_pk() const;

        int box_seal(uint8_t *ciphertext, size_t ciphertext_len,
                     const uint8_t *msg, size_t msg_len, const uint8_t *recv_pk);

        int box_seal_open(uint8_t *msg, size_t msg_len, const uint8_t *ciphertext, size_t ciphertext_len);

        int sign_detached(unsigned char *sig,
                          const unsigned char *msg, size_t msg_len);

        static int sign_verify_detached(const uint8_t *sig, size_t sig_len,
                                        const uint8_t *msg, size_t msg_len,
                                        const uint8_t *signer_pubkey);

        static size_t get_signed_message_len(size_t msg_len);

        static int sign_with_message(unsigned char *sig, unsigned long long &sig_len,
                                     const unsigned char *msg, size_t msg_len,
                                     const uint8_t *signer_sk);

        static int sign_open_with_message(const uint8_t *sig, size_t sig_len,
                                          uint8_t *msg, unsigned long long &msg_len,
                                          const uint8_t *signer_pubkey);

        static size_t get_box_easy_cipher_len(size_t msg_len);

        static size_t get_box_seal_cipher_len(size_t msg_len);

        /**
         * Modifies nonce to store the actual nonce random value.
         * @param ciphertext
         * @param ciphertext_len
         * @param msg
         * @param msg_len
         * @param nonce
         * @param receiver_pubkey
         * @return
         */
        int box_easy(uint8_t *ciphertext, size_t ciphertext_len, const uint8_t *msg, size_t msg_len,
                     uint8_t *nonce, const uint8_t *receiver_pubkey);

        int box_open_easy(uint8_t *msg, size_t msg_len, const uint8_t *ciphertext, size_t ciphertext_len,
                          const uint8_t *nonce, const uint8_t *sender_pubkey);
    };
}

#endif //LIBTOT_ASYMMETRICENCRYPTIONKEYSET_HPP
