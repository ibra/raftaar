#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// after any of the typing tests from any of the modes complete itll just pass data to this function
// so it can write to it. and the data for one test is going to be separated by commas.

void save_test_data(const std::string &filename, const std::vector<std::vector<std::string>> &data)
{
    std::ofstream file(filename, std::ios::app);
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

// here it reads the data from the file and returns it as a 2D vector of strings

std::vector<std::vector<std::string>> load_test_data(const std::string &filename)
{
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}
