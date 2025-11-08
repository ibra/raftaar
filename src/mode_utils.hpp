#include <string>
#include <vector>
#include <chrono>

double calculate_wpm(int correct_words,
                     std::chrono::steady_clock::time_point start,
                     std::chrono::steady_clock::time_point end);

std::vector<std::string> get_random_words(int count);
