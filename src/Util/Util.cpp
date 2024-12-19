#include "Util.h"

#include <fstream>
#include <iostream>

std::string read_file_to_string(const std::filesystem::path& file_path)
{
    std::ifstream in_file(file_path);

    if (!in_file)
    {
        std::cerr << "Failed to open file " << file_path << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(in_file)),
                        (std::istreambuf_iterator<char>()));

    return content;
}

std::vector<std::string> split_string(const std::string& string, char delim)
{
    std::vector<std::string> tokens;

    std::stringstream stream(string);
    std::string token;
    while (std::getline(stream, token, delim))
    {
        tokens.push_back(token);
    }
    return tokens;
}