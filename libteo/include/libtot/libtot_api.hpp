#ifndef LIBTOT_LIBTOT_API_H
#define LIBTOT_LIBTOT_API_H

#include <unordered_map>

#include "libtot_helper.hpp"
#include "libtot_logger.hpp"

namespace libtot
{

    /*
     * Library internal variables
     */

    extern bool instance_initialized;

    /*
     * External API interfaces
     */

    /**
     * Load default configurations upon initialization.
     * @return
     */
    int api_initialize();

    bool api_libtot_initialized();

    /*
     * Testing methods
     */
    extern std::unordered_map<UUID, std::string, UUIDHasher> sim_storage;

    void test_ec();

    void test_ec_elligator();
}

#endif // LIBTOT_LIBTOT_API_H