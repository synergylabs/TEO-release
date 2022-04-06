//
// Created by han on 2/18/21.
//

#ifndef LIBTOT_PUBLICSECRETKEYPAIR_HPP
#define LIBTOT_PUBLICSECRETKEYPAIR_HPP

#include <sodium.h>
#include <string>

namespace libtot {
    class PublicSecretKeyPair {
    private:
        static int deterministic_seed_counter;

        bool initialized = false;

        // pk and sk cannot be initialized due to their dynamic size
        unsigned char *pk = nullptr;
        unsigned char *sk = nullptr;

        bool valid_pk = false;
        bool valid_sk = false;

        int generate_seed_keypair(const unsigned char *seed);

        void allocate_keypair();

    public:
        PublicSecretKeyPair() = default;

        void initialize();

        void initialize(const unsigned char *seed);

        void initialize(const unsigned char *pk_in, size_t pk_in_len,
                        const unsigned char *sk_in, size_t sk_in_len);

        void initialize(const unsigned char *pk_in, size_t pk_in_len);

        void initialize(const std::string &pk_in);

        void release();

        virtual ~PublicSecretKeyPair();

        void reset_keypair();

        void set_pk(const unsigned char *pk_in, size_t pk_in_len);

        void set_sk(const unsigned char *sk_in, size_t sk_in_len);

        [[nodiscard]] const unsigned char *get_pk() const;

        [[nodiscard]] const unsigned char *get_sk() const;

        [[nodiscard]] virtual size_t get_pk_size() const = 0;

        [[nodiscard]] virtual size_t get_sk_size() const = 0;

        [[nodiscard]] virtual size_t get_seed_size() const = 0;

        virtual int crypto_seed_keypair_generator(unsigned char *pk,
                                                  unsigned char *sk,
                                                  const unsigned char *seed) = 0;

        [[nodiscard]] bool has_valid_pk() const;

        [[nodiscard]] bool has_valid_sk() const;

        [[nodiscard]] bool is_initialized() const;
    };
}

#endif //LIBTOT_PUBLICSECRETKEYPAIR_HPP
