//
// Created by han on 2/11/21.
//

#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <sodium.h>
#include <sys/sendfile.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <random>

#include "teo/teo_helper.hpp"
#include "teo/teo_logger.hpp"
#include "teo/teo_params.hpp"

namespace teo
{
    void random_buf(void *const buf, const size_t buf_len)
    {
        randombytes_buf(buf, buf_len);
    }

    std::string random_string(const size_t str_len)
    {
        std::string res;
        res.resize(G_BEACON_MSG_LEN, 'a');
        for (int i = 0; i < res.size(); i++) {
            res[i] = 'a' + randombytes_uniform('z' - 'a' + 1);
        }

        return res;
    }

    std::mutex UUID::g_uuid_mutex;

    UUID::UUID()
    {
        std::string tmp = generate_uuid_v4();
        assert(tmp.size() == UUID_SIZE);
        memcpy(buf, tmp.c_str(), UUID_SIZE);

        initialized = true;
    }

    UUID::UUID(const std::string &uuid_in)
        : UUID(uuid_in.c_str(), uuid_in.size()) {}

    UUID::UUID(const int8_t *uuid_in, size_t uuid_in_len)
        : UUID(reinterpret_cast<const char *>(uuid_in), uuid_in_len) {}

    UUID::UUID(const char *uuid_in, size_t uuid_in_len)
    {
        assert(uuid_in_len >= UUID_SIZE);
        memcpy(buf, uuid_in, UUID_SIZE);
        initialized = true;
    }

    UUID::UUID(const UUID &obj)
    {
        initialized = obj.initialized;
        memcpy(buf, obj.buf, UUID_SIZE);
    }

    UUID &UUID::operator=(const UUID &obj)
    {
        initialized = obj.initialized;
        memcpy(buf, obj.buf, UUID_SIZE);
        return *this;
    }
    std::string UUID::get_uuid() const
    {
        assert(initialized);
        return std::string(buf);
    }

    std::string UUID::generate_uuid_v4()
    {
        // https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
        std::unique_lock<std::mutex> uuid_random_lock(UUID::g_uuid_mutex, std::defer_lock);
        uuid_random_lock.lock();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 15);
        std::uniform_int_distribution<int> dis2(8, 11);

        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++)
        {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++)
        {
            ss << dis(gen);
        };

        uuid_random_lock.unlock();
        return ss.str();
    }

    bool UUID::operator==(const UUID &other) const
    {
        bool res = (initialized == other.initialized);

        res = res && (memcmp(buf, other.buf, UUID_SIZE) == 0);

        // LOGV("UUID %s vs. %s : %d", buf, other.buf, res);

        return res;
    }

    std::size_t UUIDHasher::operator()(const UUID &obj) const
    {
        using std::size_t;

        size_t res = 0;
        res = std::hash<std::string>{}(obj.get_uuid());

        // LOGV("UUID %s hash value: %lu", obj.get_uuid().c_str(), res);

        return res;
    }
}