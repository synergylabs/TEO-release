//
// Created by han on 2/15/21.
//

#ifndef TEO_SHAREDSECRETKEY_HPP
#define TEO_SHAREDSECRETKEY_HPP

#include <sodium.h>
#include <string>

namespace teo
{
    class SharedSecretKey
    {
    public:
        static const int KEY_SIZE = crypto_secretstream_xchacha20poly1305_KEYBYTES;
        static const int HEADER_SIZE = crypto_secretstream_xchacha20poly1305_HEADERBYTES;

        SharedSecretKey();

        SharedSecretKey(const uint8_t *key_in, size_t key_in_len);

        SharedSecretKey(const std::string &key_in);

        // Overload copy constructor
        SharedSecretKey(const SharedSecretKey &obj);

        // Overload assignment operator
        SharedSecretKey &operator=(const SharedSecretKey &obj);

        int encrypt(unsigned char *cipher_buf, size_t cipher_buf_len,
                    const unsigned char *msg, size_t msg_len,
                    bool final_tag = true);

        int decrypt(unsigned char *msg, size_t msg_len,
                    const unsigned char *cipher_buf, size_t cipher_buf_len);

        const unsigned char *get_key();

        const unsigned char *get_header();

        int load_header_decryption(const unsigned char *header_in, size_t header_in_len);

        int init_header_encryption(bool force = false);

        [[nodiscard]] bool is_initialized() const;

        static size_t get_cipher_len(size_t msg_len);

        static size_t get_plain_len(size_t cipher_len);

        void copy_obj(const SharedSecretKey &obj);

    private:
        enum StateStatus
        {
            UNSET,
            PULL,
            PUSH
        };

        crypto_secretstream_xchacha20poly1305_state state;
        StateStatus state_status = StateStatus::UNSET;
        unsigned char key[KEY_SIZE] = {0};
        unsigned char header[HEADER_SIZE] = {0};
        bool initialized = false;

        int initialize();
    };

}

#endif //TEO_SHAREDSECRETKEY_HPP
