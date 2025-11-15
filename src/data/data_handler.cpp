#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void save_test_data(const std::string &filename, const std::vector<std::vector<std::string>> &data)
{
    std::ofstream file(filename);
    for (const auto &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            file << row[i];
            if (i != row.size() - 1)
                file << ",";
        }
        file << "\n";
    }
}