#pragma once

#include <string>
#include <vector>

void save_test_data(const std::string &filename, const std::vector<std::vector<std::string>> &data);
std::vector<std::vector<std::string>> load_test_data(const std::string &filename);