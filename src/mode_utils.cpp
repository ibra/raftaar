#include "mode_utils.hpp"
#include <fstream>
#include <cstdlib>
#include <chrono>

double calculate_wpm(int correct_words,
                     std::chrono::steady_clock::time_point start,
                     std::chrono::steady_clock::time_point end) {
  using namespace std::chrono;
  double elapsed_seconds = duration_cast<duration<double>>(end - start).count();
  return elapsed_seconds == 0 ? 0.0 : (correct_words * 60.0) / elapsed_seconds;
}

std::vector<std::string> get_random_words(int count) {
  std::ifstream file("assets/words_alpha.txt");
  std::vector<std::string> words, result;
  std::string word;

  if (!file.is_open()) return {"error: couldn't open file."};
  while (file >> word) words.push_back(word);

  if (words.empty()) return {"error: no words found."};

  for (int i = 0; i < count; ++i)
    result.push_back(words[rand() % words.size()]);

  return result;
}
