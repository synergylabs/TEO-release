//
// Created by han on 2/19/21.
//
#include <cassert>
#include <cstring>

#include "teo/PreAuthToken.hpp"

namespace teo {
    PreAuthToken::PreAuthToken(const uint8_t *raw_token, size_t token_len) {
        assert (token_len == get_token_len());
        initialized = true;
        memcpy(token, raw_token, token_len);
    }


    bool PreAuthToken::is_initialized() const {
        return initialized;
    }

    PreAuthToken::PreAuthToken(const PreAuthToken &obj) {
        this->initialized = obj.initialized;
        memcpy(token, obj.token, get_token_len());
    }

    PreAuthToken::PreAuthToken(const SignatureKey &signer_key, const uint8_t *content, size_t content_len) {
        initialized = true;
        crypto_sign_detached(token, nullptr, content, content_len, signer_key.get_sk());
    }

    const uint8_t *PreAuthToken::get_token() {
        if (!is_initialized()) {
            return nullptr;
        }

        return token;
    }

    bool PreAuthToken::verify(const SignatureKey &singer_key, const uint8_t *content, size_t content_len) {
        if (crypto_sign_verify_detached(token,
                                        content,
                                        content_len,
                                        singer_key.get_pk()) == 0) {
            // Correct token
            return true;
        } else {
            return false;
        }
    }

    PreAuthToken &PreAuthToken::operator=(const PreAuthToken &obj) {
        this->initialized = obj.initialized;
        memcpy(token, obj.token, get_token_len());
        return *this;
    }

}