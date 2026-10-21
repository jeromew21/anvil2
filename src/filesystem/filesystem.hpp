#pragma once

#include <string>

// bool read_file(const std::string &path, std::string &out_contents);

// bool write_file(const std::string &path, const std::string &contents);

std::string path_join(const std::string& a, const std::string& b);

std::string path_to_executable_dir();
