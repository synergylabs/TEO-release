//
// Created by han on 2/23/21.
//

#include <stdexcept>
#include <cstring>
#include <iostream>
#include <cassert>

#include "libtot/libtot_logger.hpp"
#include "libtot/AsymmetricEncryptionKeySet.hpp"

namespace libtot
{

    AsymmetricEncryptionKeySet::AsymmetricEncryptionKeySet()
    {
        aead_key = PKAEADKey(nullptr);
        sign_key = SignatureKey(nullptr);
    }

    AsymmetricEncryptionKeySet::AsymmetricEncryptionKeySet(const uint8_t *pub_key_buf,
                                                           size_t pub_key_buf_len)
    {
        if (pub_key_buf_len < FULL_PK_SIZE)
        {
            throw std::runtime_error("Public key buffer too short");
        }

        aead_key = PKAEADKey(pub_key_buf + PK_BUF_AEAD_OFFSET,
                             PKAEADKey::PUBLICKEYBYTES);
        sign_key = SignatureKey(pub_key_buf + PK_BUF_SIGN_OFFSET,
                                SignatureKey::PUBLICKEYBYTES);
    }

    AsymmetricEncryptionKeySet::AsymmetricEncryptionKeySet(const uint8_t *pub_key_buf,
                                                           size_t pub_key_buf_len,
                                                           const uint8_t *priv_key_buf,
                                                           size_t priv_key_buf_len)
    {
        if (pub_key_buf_len < FULL_PK_SIZE)
        {
            throw std::runtime_error("Public key buffer too short");
        }

        if (priv_key_buf_len < FULL_SK_SIZE)
        {
            throw std::runtime_error("Private key buffer too short");
        }
        aead_key = PKAEADKey(pub_key_buf + PK_BUF_AEAD_OFFSET, PKAEADKey::PUBLICKEYBYTES,
                             priv_key_buf + SK_BUF_AEAD_OFFSET, PKAEADKey::SECRETKEYBYTES);
        sign_key = SignatureKey(pub_key_buf + PK_BUF_SIGN_OFFSET, SignatureKey::PUBLICKEYBYTES,
                                priv_key_buf + SK_BUF_SIGN_OFFSET, SignatureKey::SECRETKEYBYTES);
    }

    AsymmetricEncryptionKeySet::AsymmetricEncryptionKeySet(const AsymmetricEncryptionKeySet &obj)
    {
        aead_key = obj.aead_key;
        sign_key = obj.sign_key;
    }

    AsymmetricEncryptionKeySet &
    AsymmetricEncryptionKeySet::operator=(const AsymmetricEncryptionKeySet &obj)
    {
        aead_key = obj.aead_key;
        sign_key = obj.sign_key;

        return *this;
    }

    const PKAEADKey &AsymmetricEncryptionKeySet::get_aead_key()
    {
        return aead_key;
    }

    const SignatureKey &AsymmetricEncryptionKeySet::get_sign_key()
    {
        return sign_key;
    }

    int AsymmetricEncryptionKeySet::get_full_pk(uint8_t *buf, size_t buf_len) const
    {
        if (buf_len < FULL_PK_SIZE)
        {
            throw std::runtime_error("public key buffer too short to hold all data");
        }
        memcpy(buf + PK_BUF_AEAD_OFFSET,
               aead_key.get_pk(),
               aead_key.get_pk_size());
        memcpy(buf + PK_BUF_SIGN_OFFSET,
               sign_key.get_pk(),
               sign_key.get_pk_size());

        return 0;
    }

    std::string AsymmetricEncryptionKeySet::get_full_pk() const
    {
        std::string res;
        res.resize(FULL_PK_SIZE);
        memcpy(&res[PK_BUF_AEAD_OFFSET], aead_key.get_pk(), aead_key.get_pk_size());
        memcpy(&res[PK_BUF_SIGN_OFFSET], sign_key.get_pk(), sign_key.get_pk_size());
        return res;
    }

    int AsymmetricEncryptionKeySet::get_full_sk(uint8_t *buf, size_t buf_len) const
    {
        if (buf_len < FULL_SK_SIZE)
        {
            throw std::runtime_error("public key buffer too short to hold all data");
        }

        memcpy(buf + SK_BUF_AEAD_OFFSET,
               aead_key.get_sk(),
               aead_key.get_sk_size());
        memcpy(buf + SK_BUF_SIGN_OFFSET,
               sign_key.get_sk(),
               sign_key.get_sk_size());

        return 0;
    }

    void AsymmetricEncryptionKeySet::print_pk() const
    {
        std::cout << "AEAD public key: ";
        hexprint(aead_key.get_pk(), aead_key.get_sk_size());
        std::cout << "Signature public key: ";
        hexprint(sign_key.get_pk(), sign_key.get_pk_size());
    }

    int AsymmetricEncryptionKeySet::box_seal_open(uint8_t *msg, size_t msg_len,
                                                  const uint8_t *ciphertext,
                                                  size_t ciphertext_len)
    {
        assert(get_box_seal_cipher_len(msg_len) == ciphertext_len);
        return aead_key.box_seal_open(msg, ciphertext, ciphertext_len);
    }

    size_t AsymmetricEncryptionKeySet::get_box_easy_cipher_len(size_t msg_len)
    {
        return msg_len + PKAEADKey::MACBYTES;
    }

    int AsymmetricEncryptionKeySet::box_easy(uint8_t *ciphertext, size_t ciphertext_len,
                                             const uint8_t *msg,
                                             size_t msg_len, uint8_t *nonce,
                                             const uint8_t *receiver_pubkey)
    {
        assert(get_box_easy_cipher_len(msg_len) == ciphertext_len);
        randombytes_buf(nonce, NONCE_SIZE);
        AsymmetricEncryptionKeySet receiver_key(receiver_pubkey);
        return aead_key.box_easy(ciphertext, msg, msg_len, nonce,
                                 receiver_key.get_aead_key().get_pk());
    }

    size_t AsymmetricEncryptionKeySet::get_box_seal_cipher_len(size_t msg_len)
    {
        return msg_len + PKAEADKey::SEALBYTES;
    }

    int
    AsymmetricEncryptionKeySet::box_seal(uint8_t *ciphertext, size_t ciphertext_len,
                                         const uint8_t *msg, size_t msg_len,
                                         const uint8_t *recv_pk)
    {
        assert(ciphertext_len == get_box_seal_cipher_len(msg_len));

        AsymmetricEncryptionKeySet receiver_key(recv_pk);

        return crypto_box_seal(ciphertext, msg, msg_len, receiver_key.get_aead_key().get_pk());
    }

    int AsymmetricEncryptionKeySet::box_open_easy(uint8_t *msg, size_t msg_len,
                                                  const uint8_t *ciphertext,
                                                  size_t ciphertext_len, const uint8_t *nonce,
                                                  const uint8_t *sender_pubkey)
    {
        assert(ciphertext_len == get_box_easy_cipher_len(msg_len));
        AsymmetricEncryptionKeySet sender_key(sender_pubkey);
        return aead_key.box_open_easy(msg, ciphertext, ciphertext_len, nonce,
                                      sender_key.get_aead_key().get_pk());
    }

    int AsymmetricEncryptionKeySet::sign_detached(unsigned char *sig,
                                                  const unsigned char *msg,
                                                  size_t msg_len)
    {
        return sign_key.sign_detached(sig, msg, msg_len);
    }

    int AsymmetricEncryptionKeySet::sign_verify_detached(const uint8_t *sig, size_t sig_len,
                                                         const uint8_t *msg, size_t msg_len,
                                                         const uint8_t *signer_pubkey)
    {
        assert(sig_len == SIGNATURE_SIZE);
        AsymmetricEncryptionKeySet signer_key(signer_pubkey);
        return crypto_sign_verify_detached(sig, msg, msg_len, signer_key.get_sign_key().get_pk());
    }

    size_t AsymmetricEncryptionKeySet::get_signed_message_len(size_t msg_len)
    {
        return crypto_sign_BYTES + msg_len;
    }

    int AsymmetricEncryptionKeySet::sign_with_message(unsigned char *sig, unsigned long long &sig_len,
                                                      const unsigned char *msg, size_t msg_len,
                                                      const uint8_t *signer_sk)
    {
        assert(sig_len >= get_signed_message_len(msg_len));
        return crypto_sign(sig, &sig_len, msg, msg_len, signer_sk);
    }

    int AsymmetricEncryptionKeySet::sign_open_with_message(const uint8_t *sig, size_t sig_len,
                                                           uint8_t *msg, unsigned long long &msg_len,
                                                           const uint8_t *signer_pubkey)
    {
        return crypto_sign_open(msg, &msg_len,
                                sig, sig_len, signer_pubkey);
    }

    AsymmetricEncryptionKeySet::AsymmetricEncryptionKeySet(const uint8_t *pub_key)
        : AsymmetricEncryptionKeySet(pub_key, FULL_PK_SIZE) {}

}
