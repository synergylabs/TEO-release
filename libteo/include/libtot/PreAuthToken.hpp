//
// Created by han on 2/19/21.
//

#ifndef LIBTOT_PREAUTHTOKEN_HPP
#define LIBTOT_PREAUTHTOKEN_HPP

#include <sodium.h>

#include "AsymmetricEncryptionKeySet.hpp"

namespace libtot {
    class PreAuthToken {
    public:
        static const int PRE_AUTH_TOKEN_SIZE = AsymmetricEncryptionKeySet::SIGNATURE_SIZE;

        PreAuthToken() = default;

        explicit PreAuthToken(const uint8_t *raw_token, size_t token_len);

        PreAuthToken(const SignatureKey &signer_key, const uint8_t *content, size_t content_len);

        // Overload copy constructor
        PreAuthToken(const PreAuthToken &obj);

        // Overload assignment operator
        PreAuthToken &operator=(const PreAuthToken &obj);

        bool is_initialized() const;

        const uint8_t *get_token();

        static const int get_token_len() { return PRE_AUTH_TOKEN_SIZE; }

        bool verify(const SignatureKey &singer_key, const uint8_t *content, size_t content_len);

    private:
        bool initialized = false;
        uint8_t token[PRE_AUTH_TOKEN_SIZE] = {0};

    };
}

#endif //LIBTOT_PREAUTHTOKEN_HPP
