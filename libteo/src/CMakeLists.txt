# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${libtot_SOURCE_DIR}/include/libtot/*.hpp")

set(LIBTOT_CORE_SOURCE 
        AsymmetricEncryptionKeySet.cpp 
        base64.cpp
        client/Accessor.cpp
        client/Admin.cpp 
        client/Client.cpp
        client/Device.cpp 
        client/User.cpp 
        libtot_api.cpp 
        libtot_client_native.cpp
        libtot_file_io.cpp
        libtot_helper.cpp 
        libtot_logger.cpp 
        libtot_network.cpp
        PKAEADKey.cpp 
        PreAuthToken.cpp 
        PublicSecretKeyPair.cpp 
        SharedSecretKey.cpp
        Sieve.cpp 
        SignatureKey.cpp
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(libtot ${HEADER_LIST} 
        ${LIBTOT_CORE_SOURCE}
        unit_test.cpp
        )

# We need this directory, and users of our library will need it too
target_include_directories(libtot PUBLIC ../include)

# This depends on Decaf (for Ed448), libsodium, libconfig++, pthread, CryptoPP (static lib)
target_link_libraries(libtot PRIVATE decaf sodium config++ pthread libcryptopp.a)

# All users of this library will need at least C++11
target_compile_features(libtot PUBLIC cxx_std_17)


# Make an automatic library - will be static or dynamic based on user setting
add_library(libtot-storage ${HEADER_LIST} 
        ${LIBTOT_CORE_SOURCE}
        client/Storage.cpp
        unit_test.cpp
        )

# We need this directory, and users of our library will need it too
target_include_directories(libtot-storage PUBLIC ../include)

# This depends on Decaf (for Ed448), libsodium, libconfig++, pthread, CryptoPP (static lib), LevelDB (and its dependency on Snappy lib)
target_link_libraries(libtot-storage PRIVATE decaf sodium config++ pthread libcryptopp.a libleveldb.a snappy)

# All users of this library will need at least C++11
target_compile_features(libtot-storage PUBLIC cxx_std_11)


# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
