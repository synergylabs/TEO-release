//
// Created by han on 10/30/20.
//
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <algorithm>

#include "teo/teo_helper.hpp"
#include "teo/teo_logger.hpp"
#include "teo/Sieve.hpp"

namespace teo
{
    using decaf::Block;
    using decaf::FixedBlock;
    using decaf::SecureBuffer;
    using decaf::SHA3;
    using decaf::SpongeRng;

    typedef typename TEO_EC_Group::Scalar Scalar;
    typedef typename TEO_EC_Group::Point Point;

    const int SHA3_512 = 512;

    SieveKey::SieveKey()
    {
        key = sieve_keygen(decaf::SpongeRng::RANDOM);

        random_buf(nonce, sizeof(nonce));
    }

    SieveKey::SieveKey(const uint8_t *key_buf, size_t key_buf_len)
        : SieveKey()
    {
        assert(key_buf_len == SIEVE_KEY_SIZE);
        Block b(key_buf, key_buf_len);
        key = Scalar(b);
    }

    SieveKey::SieveKey(const uint8_t *key_buf, size_t key_buf_len,
                       const uint8_t *nonce_buf, size_t nonce_buf_len)
        : SieveKey(key_buf, key_buf_len)
    {
        set_nonce(nonce_buf, nonce_buf_len);
    }

    void SieveKey::clone(const SieveKey &obj)
    {
        key = obj.key;
        memcpy(nonce, obj.nonce, sizeof(nonce));
    }

    SieveKey::SieveKey(const SieveKey &obj)
    {
        clone(obj);
    }

    SieveKey &SieveKey::operator=(const SieveKey &obj)
    {
        clone(obj);
        return *this;
    }

    int SieveKey::set_nonce(const uint8_t *nonce_buf, size_t nonce_buf_len)
    {
        assert(nonce_buf_len == SIEVE_NONCE_SIZE);
        memcpy(nonce, nonce_buf, sizeof(nonce));
        return 0;
    }

    int SieveKey::serialize_key_into(unsigned char *buf, size_t buf_len)
    {
        assert(buf_len == SIEVE_KEY_SIZE);
        key.serialize_into(buf);
        return 0;
    }

    int SieveKey::serialize_nonce_into(unsigned char *buf, size_t buf_len)
    {
        assert(buf_len == SIEVE_NONCE_SIZE);
        memcpy(buf, nonce, buf_len);
        return 0;
    }

    bool _validate_k(const Scalar &k)
    {
        return (k != 0 && k != 1);
    }

    Point SieveKey::sieve_f_k_x(TEO_EC_Group::Scalar k, unsigned int nonce_offset, const uint8_t *nonce)
    {
        if (!_validate_k(k))
        {
            throw std::invalid_argument("Invalid k");
        }

        // Calculate N + j. Tricky because N is an uint8_t array,
        // but j is an unsigned int.
        uint8_t sum_n_j[SIEVE_NONCE_SIZE]{0};
        assert(SIEVE_NONCE_SIZE >= sizeof(nonce_offset));
        size_t trailing_len = sizeof(nonce_offset);
        size_t leading_len = SIEVE_NONCE_SIZE - trailing_len;
        memcpy(sum_n_j, nonce, leading_len);
        unsigned int trailing_sum = nonce_offset + (unsigned int)nonce[leading_len];
        memcpy(sum_n_j + leading_len, reinterpret_cast<uint8_t *>(&trailing_sum), trailing_len);

        Block block_n_j = Block(sum_n_j, SIEVE_NONCE_SIZE);
        SecureBuffer hash_n_j = SHA3<SHA3_512>::hash(block_n_j);
        Point point_h_n_j = Point::from_hash(hash_n_j);
        Point f_k_n_j = k * point_h_n_j;
        return f_k_n_j;
    }

    int SieveKey::encrypt(const uint8_t *msg, size_t msg_len, std::vector<int> &hints, SecureBuffer &cipher)
    {
        assert(hints.empty() && cipher.empty());
        size_t chunks = ceil(msg_len / (double)CHUNK_SIZE);
        for (int i = 0; i < chunks; i++)
        {
            size_t idx = i * CHUNK_SIZE;
            size_t seg_len = std::min((size_t)CHUNK_SIZE, msg_len - idx);
            SecureBuffer m(msg + idx, msg + idx + seg_len);
            m.resize(CHUNK_SIZE);

            Point p_m = Point::from_hash(m);
            bool found_hint = false;
            for (int hint = 0; hint < 10 && !found_hint; hint++)
            {
                SecureBuffer recover_m(Point::HASH_BYTES);
                p_m.invert_elligator(recover_m, hint);
                if (memeq(recover_m, m))
                {
                    hints.push_back(hint);
                    found_hint = true;
                }
            }
            assert(found_hint);

            SecureBuffer seg_cipher = p_m + sieve_f_k_x(key, i, nonce);
            cipher.insert(cipher.end(), seg_cipher.begin(), seg_cipher.end());
        }

        return 0;
    }

    const uint8_t *SieveKey::get_nonce()
    {
        return nonce;
    }

    int SieveKey::decrypt(const uint8_t *cipher, size_t cipher_len, uint8_t *msg,
                          const std::vector<int> &hints)
    {
        size_t chunks = hints.size();
        assert(chunks != 0 && cipher_len == chunks * CHUNK_SIZE);

        for (int i = 0; i < chunks; i++)
        {
            size_t idx = i * CHUNK_SIZE;
            SecureBuffer c(cipher + idx, cipher + idx + CHUNK_SIZE);

            Point p_c(c);
            Point p_m = p_c - sieve_f_k_x(key, i, nonce);
            SecureBuffer recover_m(CHUNK_SIZE);
            p_m.invert_elligator(recover_m, hints[i]);
            memcpy(msg + idx, &recover_m[0], CHUNK_SIZE);
        }

        return 0;
    }

    SecureBuffer test_sieve_prf(uint32_t k, const char *x)
    {
        return test_sieve_prf(Scalar(k), x);
    }

    SecureBuffer test_sieve_prf(const Scalar &k, const char *x)
    {
        if (!_validate_k(k))
        {
            throw std::invalid_argument("Invalid k");
        }

        SecureBuffer hash_val = SHA3<SHA3_512>::hash(Block(x));
        Point hp = Point::from_hash(hash_val);
        Point f = Scalar(k) * hp;

#ifndef NDEBUG
        std::cout << "In debugging mode!!" << std::endl;
        print("k", k);
        std::cout << "  x = " << x << std::endl;
        hexprint("hash(x)", hash_val);
        print("converted hp from hash(x)", hp);
        print("f(k, x)", f);
#endif

        return f.serialize();
    }

    Scalar sieve_keygen(SpongeRng::Deterministic det)
    {
        SpongeRng rng("/dev/urandom");

        Scalar k(rng);
        while (!_validate_k(k))
        {
            k = Scalar(rng);
        }

        return k;
    }

    SecureBuffer test_sieve_encrypt_block(const std::string &message, const Point &fp)
    {
        assert(message.size() < Point::SER_BYTES);
        SecureBuffer m(message.begin(), message.end());
        m.resize(Point::HASH_BYTES);
        hexprint("message buffer", m);

        Point mp = Point::from_hash(m);
        print("mp", mp);

        for (int hint = 0; hint < 10; hint++)
        {
            std::cout << "Hint: " << hint << std::endl;
            SecureBuffer buffer(Point::HASH_BYTES);
            mp.invert_elligator(buffer, hint);
            hexprint("mp invert elligator", buffer);
        }

        return m;
    }

    RekeyToken SieveKey::gen_rekey_token(SieveKey &new_key)
    {
        if (key != new_key.key)
        {
            return key - new_key.key;
        }
        else
        {
            LOGE("RekeyToken generation error: same key used twice");
        }
    }

    int SieveKey::apply_rekey_token_cipher(uint8_t *cipher, size_t cipher_len,
                                           const std::vector<int> &hints, RekeyToken token,
                                           const uint8_t *nonce)
    {
        size_t chunks = hints.size();
        assert(chunks != 0 && cipher_len == chunks * CHUNK_SIZE);

        for (int i = 0; i < chunks; i++)
        {
            size_t idx = i * CHUNK_SIZE;
            SecureBuffer c(cipher + idx, cipher + idx + CHUNK_SIZE);

            Point p_c(c);
            SecureBuffer seg_new = p_c + sieve_f_k_x(token, i, nonce);
            memcpy(cipher + idx, &seg_new[0], CHUNK_SIZE);
        }

        return 0;
    }

    int SieveKey::apply_rekey_token_key(RekeyToken token)
    {
        key += token;
        return 0;
    }

} // namespace teo
