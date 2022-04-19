//
// Created by han on 2/15/21.
//

#include <stdexcept>
#include <cassert>
#include <cstring>

#include "teo/teo_logger.hpp"
#include "teo/SharedSecretKey.hpp"


namespace teo {

    SharedSecretKey::SharedSecretKey() {
        initialize();
    }

    SharedSecretKey::SharedSecretKey(const uint8_t *key_in, size_t key_in_len) : SharedSecretKey() {
        assert(key_in_len == KEY_SIZE);
        memcpy(key, key_in, sizeof(key));
    }

    SharedSecretKey::SharedSecretKey(const std::string &key_in) : SharedSecretKey(
            reinterpret_cast<const uint8_t *>(key_in.c_str()), key_in.size()) {}

    SharedSecretKey::SharedSecretKey(const SharedSecretKey &obj) {
        copy_obj(obj);
    }

    SharedSecretKey &SharedSecretKey::operator=(const SharedSecretKey &obj) {
        if (this == &obj) {
            return *this;
        }
        copy_obj(obj);
        return *this;
    }

    void SharedSecretKey::copy_obj(const SharedSecretKey &obj) {
        state = obj.state;
        state_status = obj.state_status;
        memcpy(key, obj.key, KEY_SIZE);
        memcpy(header, obj.header, HEADER_SIZE);
        initialized = obj.initialized;
    }

    int SharedSecretKey::initialize() {
        if (initialized) {
            throw std::runtime_error("Key already initialized!");
        }

        state = crypto_secretstream_xchacha20poly1305_state{};
        state_status = StateStatus::UNSET;
        crypto_secretstream_xchacha20poly1305_keygen(key);
        memset(header, 0, HEADER_SIZE);
        initialized = true;

        return 0;
    }

    bool SharedSecretKey::is_initialized() const {
        return initialized;
    }

    const unsigned char *SharedSecretKey::get_key() {
        assert(is_initialized());
        return key;
    }

    const unsigned char *SharedSecretKey::get_header() {
        // It only makes sense to fetch header value after successful encryption
        assert(state_status == StateStatus::PUSH);
        return header;
    }

    int
    SharedSecretKey::load_header_decryption(const unsigned char *header_in, size_t header_in_len) {
        assert(header_in_len >= HEADER_SIZE);

        if (state_status != StateStatus::UNSET) {
            LOGV("Reset header value in dirty state");
            state_status = StateStatus::UNSET;
        }

        memcpy(header, header_in, header_in_len);

        if (crypto_secretstream_xchacha20poly1305_init_pull(&state, header, key) != 0) {
            LOGE("Failed to initialize state from header and key");
        }

        state_status = StateStatus::PULL;

        return 0;
    }

    size_t SharedSecretKey::get_cipher_len(size_t msg_len) {
        return msg_len + crypto_secretstream_xchacha20poly1305_ABYTES;
    }

    int SharedSecretKey::encrypt(unsigned char *cipher_buf, size_t cipher_buf_len,
                                 const unsigned char *msg, size_t msg_len,
                                 bool final_tag) {
        assert(get_cipher_len(msg_len) == cipher_buf_len);

        if (state_status == StateStatus::UNSET) {
            init_header_encryption();
        } else if (state_status != StateStatus::PUSH) {
            LOGV("Dirty internal state in encryption!!");
            assert(false);
        }

        unsigned char tag = 0;
        if (final_tag) {
            tag = crypto_secretstream_xchacha20poly1305_TAG_FINAL;
        }

        crypto_secretstream_xchacha20poly1305_push(&state, cipher_buf, nullptr,
                                                   msg, msg_len,
                                                   nullptr, 0,
                                                   tag);
        return 0;
    }

    int
    SharedSecretKey::decrypt(unsigned char *msg, size_t msg_len, const unsigned char *cipher_buf,
                             size_t cipher_buf_len) {
        if (msg_len != 0) {
            assert(get_cipher_len(msg_len) == cipher_buf_len);
        }

        if (state_status != StateStatus::PULL) {
            LOGV("Unexpected internal state. Not in decryption mode");
            return -1;
        }

        unsigned char tag;
        if (crypto_secretstream_xchacha20poly1305_pull(&state, msg, nullptr, &tag, cipher_buf,
                                                       cipher_buf_len,
                                                       nullptr, 0) != 0) {
            throw "Decryption failed!";
        }

        return 0;
    }

    int SharedSecretKey::init_header_encryption(bool force) {
        if (!force) {
            assert(state_status == StateStatus::UNSET);
        }
        crypto_secretstream_xchacha20poly1305_init_push(&state, header, key);
        state_status = StateStatus::PUSH;
        return 0;
    }

    size_t SharedSecretKey::get_plain_len(size_t cipher_len) {
        assert(cipher_len > crypto_secretstream_xchacha20poly1305_ABYTES);
        return cipher_len - crypto_secretstream_xchacha20poly1305_ABYTES;
    }

}
