#ifndef TEO_TEO_API_H
#define TEO_TEO_API_H

#include <unordered_map>

#include "teo_helper.hpp"
#include "teo_logger.hpp"

namespace teo
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

    bool api_teo_initialized();

    void test_ec();

    void test_ec_elligator();
}

#endif // TEO_TEO_API_H