// Auxiliary helper functions.
//
// Created by han on 2/10/21.
//

#ifndef TEO_TEO_HELPER_HPP
#define TEO_TEO_HELPER_HPP

#include <cstddef>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>


namespace teo
{
    void random_buf(void *buf, size_t buf_len);
    std::string random_string(size_t str_len);

    class UUID
    {
    public:
        static const int UUID_SIZE = 36;
        static std::mutex g_uuid_mutex;
        
        UUID();

        explicit UUID(const std::string &uuid_in);

        UUID(const int8_t *uuid_in, size_t uuid_in_len);

        UUID(const char *uuid_in, size_t uuid_in_len);

        // Overload copy constructor
        UUID(const UUID &obj);

        // Overload assignment operator
        UUID &operator=(const UUID &obj);

        std::string generate_uuid_v4();

        std::string get_uuid() const;

        bool operator==(const UUID &other) const;

    private:
        bool initialized = false;

        char buf[UUID_SIZE+1]{}; // including null terminator for c-str
    };

    struct UUIDHasher
    {
        std::size_t operator()(const UUID &obj) const;
    };
}

#endif //TEO_TEO_HELPER_HPP
