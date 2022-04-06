//
// Created by han on 2/11/21.
//

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "libtot/libtot_logger.hpp"

namespace libtot
{
    void hexprint(const unsigned char *buf, size_t length, int indent)
    {
        if (length == 0)
        {
            return;
        }

        std::stringstream ss;

        while (indent > 0) {
            ss << "\t";
            indent--;
        }

        ss << "0x";
        for (int i = 0; i < length; i++)
        {
            ss << std::setfill('0') << std::setw(2)
               << std::right << std::hex << (unsigned int)buf[i];
        }
        LOGV("%s", ss.str().c_str());
    }

    void hexprint(const char *buf, size_t length, int indent )
    {
        return hexprint(reinterpret_cast<const unsigned char *>(buf), length, indent);
    }

    void hexprint(const char *name, const decaf::SecureBuffer &buffer)
    {
        std::stringstream ss;

        ss << "  " << name << " = 0x";

        for (int i = buffer.size() - 1; i >= 0; i--)
        {
            ss << std::setfill('0') << std::setw(2)
               << std::right << std::hex << (unsigned int)buffer[i];
        }
        LOGV("%s", ss.str().c_str());
    }

    static std::string log_level_name[__MAX_LEVELS] = {
        "ERROR",
        "WARN",
        "INFO",
        "DEBUG",
        "VERBOSE",
    };

    char *strip(char *time)
    {
        time[strlen(time) - 1] = '\0'; // null the '\n' char at the end
        return time;
    }

    void _log(const std::string tag, log_level level, const char *fmt, ...)
    {
        va_list args;
        time_t t;
        if (level <= LOG_LEVEL)
        {
            t = time(NULL); // now
            // print log prefix
            fprintf(STREAM, "%s %s | %s: ", strip(asctime(localtime(&t))), tag.c_str(), log_level_name[level].c_str());
            // print message
            va_start(args, fmt);
            vfprintf(STREAM, fmt, args);
            // newline at the end
            fprintf(STREAM, "\n");
        }
        fflush(STREAM);
    }

}