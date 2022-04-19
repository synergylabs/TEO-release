//
// Created by han on 2/11/21.
//

#ifndef TEO_TEO_LOGGER_HPP
#define TEO_TEO_LOGGER_HPP

#include <decaf.hxx>
#include <decaf/spongerng.hxx>
#include <string>

namespace teo {

#if !defined(TEO_STANDALONE_APP)

#define LOGE(args...) //_log(TAG, ERROR, args)
#define LOGW(args...) //_log(TAG, WARN, args)
#define LOGI(args...) //_log(TAG, INFO, args)
#define LOGD(args...) //_log(TAG, DEBUG, args)
#define LOGV(args...) //_log(TAG, VERBOSE, args)

#define hexprint(args...) //
#define print(args...) //

#else

    const char TAG[] = "teo logging";

    void hexprint(const unsigned char *buf, size_t length, int indent = 0);

    void hexprint(const char *buf, size_t length, int indent = 0);

    void hexprint(const char *name, const decaf::SecureBuffer &buffer);

    // We must put this implementation in the header file,
    // because it is a template function :(
    template <typename PointOrScalar>
    void print(const char *name, const PointOrScalar &x)
    {
        unsigned char buffer[PointOrScalar::SER_BYTES];
        x.serialize_into(buffer);
        printf("  %s = 0x", name);
        for (int i = PointOrScalar::SER_BYTES - 1; i >= 0; i--)
        {
            printf("%02x", buffer[i]);
        }
        printf("\n");
    }

// convenience macro to prepend TAG macro argument
#define LOG(args...) _log(TAG, args)
// convenience macros to prepend TAG and log level
//#define LOGE(args...) _log(TAG,ERROR,args)
#define LOGE(args...)           \
    {                           \
        _log(TAG, ERROR, args); \
        exit(EXIT_FAILURE);     \
    }
#define LOGW(args...) _log(TAG, WARN, args)
#define LOGI(args...) _log(TAG, INFO, args)
#define LOGD(args...) _log(TAG, DEBUG, args)
#define LOGV(args...) _log(TAG, VERBOSE, args)

    typedef enum
    {
        ERROR,   // program cannot continue
        WARN,    // program did something strange, but can continue
        INFO,    // program did something noteworthy
        DEBUG,   // used for development debugging
        VERBOSE, // status messages
        __MAX_LEVELS
    } log_level;

    char *strip(char *time);

    void _log(std::string tag, log_level level, const char *fmt, ...);

// GLOBAL VARS
#if !defined(LOG_LEVEL)
#if !defined(NDEBUG)
#define LOG_LEVEL VERBOSE
#else
#define LOG_LEVEL INFO
#endif // NDEBUG
#endif // LOG_LEVEL

#define STREAM stdout

#endif

}

#endif //TEO_TEO_LOGGER_HPP
