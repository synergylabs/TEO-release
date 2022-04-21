//
// Created by Han Zhang on 6/10/21.
//

#ifndef TOT_MOBILE_NATIVE_TOT_HELPER_HPP
#define TOT_MOBILE_NATIVE_TOT_HELPER_HPP

#include <vector>

//#include "libtot/AsymmetricEncryptionKeySet.hpp"
//#include "libtot/CipherType.hpp"
//#include "libtot/libtot_helper.hpp"
//#include "libtot/libtot_logger.hpp"
//#include "libtot/libtot_network.hpp"
//#include "libtot/libtot_params.hpp"
//#include "libtot/Message.hpp"
//#include "libtot/PKAEADKey.hpp"
//#include "libtot/PublicSecretKeyPair.hpp"
//#include "libtot/SharedSecretKey.hpp"
//#include "libtot/Sieve.hpp"
//#include "libtot/SignatureKey.hpp"
//#include "libtot/SignatureType.hpp"

namespace libtot {
    int generate_keypair(std::vector<int8_t> &pubkey, std::vector<int8_t> &privkey);
}

#endif //TOT_MOBILE_NATIVE_TOT_HELPER_HPP
