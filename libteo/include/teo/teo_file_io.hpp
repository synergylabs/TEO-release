// Auxiliary helper functions.
//
// Created by han on 2/10/21.
//

#ifndef TEO_TEO_FILE_IO_HPP
#define TEO_TEO_FILE_IO_HPP

#include <cstddef>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "teo/teo_helper.hpp"

namespace teo
{
    int write_to_file(const std::string &output_file_path, const uint8_t *file_encrypted,
                      size_t file_encrypted_len, bool append = false);

    int read_from_file(const std::string &input_file_path, char **buf_ptr, size_t *buf_len_ptr);

    int upload_file(int conn, const UUID &uuid, const std::string &file_path, std::vector<const uint8_t*> owner_keys);

    int upload_content(int conn, const UUID &uuid, const uint8_t *data_buf, size_t data_buf_len, std::vector<const uint8_t*> owner_keys);

    int download_file(int conn, const UUID &uuid, char ** buf_ptr, size_t *buf_len_ptr);
}

#endif //TEO_TEO_FILE_IO_HPP
