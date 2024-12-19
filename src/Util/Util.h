#pragma once

#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>


std::string read_file_to_string(const std::filesystem::path& file_path);
std::vector<std::string> split_string(const std::string& string, char delim = ' ');
