//
// Created by han on 2/11/21.
//

#include <libconfig.h++>
#include <unistd.h>
#include <pwd.h>
#include <iostream>

#include "teo/teo_api.hpp"

namespace teo {
    using libconfig::Config;
    using libconfig::FileIOException;
    using libconfig::ParseException;
    using libconfig::SettingNotFoundException;

    const char CONFIG_FILENAME[] = "teo_user_config.cfg";

    bool instance_initialized = false;

    // Instantiate the config variables
    std::string default_device_ip;
    unsigned short default_device_port = 0;
    std::string default_admin_ip;
    unsigned short default_admin_port = 0;
    std::string default_storage_ip;
    unsigned short default_storage_port = 0;
    std::string accessor_ip;
    unsigned short accessor_port = 0;

    std::string storage_content_location;
    std::string storage_owner_location;

    bool api_teo_initialized() {
        return instance_initialized;
    }

    void load_config_file(std::string &config_file_full_path) {
        LOGV("Loading libconfig configuration file...");

        Config cfg;

        try {
            cfg.readFile(config_file_full_path.c_str());
        }
        catch (const FileIOException &fioex) {
            std::cerr << "I/O error while reading file." << std::endl;
            exit(EXIT_FAILURE);
        }
        catch (const ParseException &pex) {
            std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                      << " - " << pex.getError() << std::endl;
            exit(EXIT_FAILURE);
        }

        try {
            cfg.lookupValue("device_ip", default_device_ip);
            cfg.lookupValue("admin_ip", default_admin_ip);
            cfg.lookupValue("storage_ip", default_storage_ip);
            cfg.lookupValue("accessor_ip", accessor_ip);

            int conversion = 0;
            cfg.lookupValue("device_port", conversion);
            default_device_port = (short)conversion;
            cfg.lookupValue("admin_port", conversion);
            default_admin_port = (short)conversion;
            cfg.lookupValue("storage_port", conversion);
            default_storage_port = (short)conversion;
            cfg.lookupValue("accessor_port", conversion);
            accessor_port = (short)conversion;

            cfg.lookupValue("storage_content_location", storage_content_location);
            cfg.lookupValue("storage_owner_location", storage_owner_location);
        }
        catch (const SettingNotFoundException &nfex) {
            std::cerr << "Missing setting configuration fields" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    int api_initialize() {
        // Get config file location
        const char *homedir;

        if ((homedir = getenv("HOME")) == nullptr) {
            homedir = getpwuid(getuid())->pw_dir;
        }

        if (homedir == nullptr) {
            throw std::runtime_error("Cannot detect user's home location");
        }

        std::string config_file_full_path(homedir);
        config_file_full_path.append("/" + std::string(CONFIG_FILENAME));

        load_config_file(config_file_full_path);

        instance_initialized = true;

        return 0;
    }

}