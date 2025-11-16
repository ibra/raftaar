#include "utils/mode_utils.hpp"
#include "data/data_handler.hpp"

#include <fstream>
#include <cstdlib>
#include <chrono>

// calculates the words per minute (wpm) based on the number of correct words typed and the elapsed time

double calculate_wpm(int correct_chars,
                     std::chrono::steady_clock::time_point start,
                     std::chrono::steady_clock::time_point end)
{
  using namespace std::chrono;
  double elapsed_seconds = duration_cast<duration<double>>(end - start).count();

  // when the elapsed time is less than a second, or less than 2 correct words, return 0 to avoid inflated wpm
  if (elapsed_seconds < 1.0)
  {
    return 0.0;
  }

  double minutes = elapsed_seconds / 60.0;
  double wpm = (correct_chars / 5.0) / minutes;

  return wpm;
}

// the word file here was in the public domain and was sourced from
// https://github.com/MichaelWehar/Public-Domain-Word-Lists/blob/master/5000-more-common.txt

std::vector<std::string> get_random_words(int count)
{
  std::ifstream file("assets/words_alpha.txt");
  std::vector<std::string> words, result;
  std::string word;

  if (!file.is_open())
    return {"error: couldn't open file."};
  while (file >> word)
    words.push_back(word);

  if (words.empty())
    return {"error: no words found."};

  for (int i = 0; i < count; ++i)
    result.push_back(words[rand() % words.size()]);

  return result;
}

// handles the space key event during typing tests. if the space key gets pressed
// and there arent any words left then it saves it to a file permanently with the data handler.

bool handle_space_key(
    TypingState &state,
    int max_items,
    ScreenInteractive &screen)
{
  if (state.current_index >= max_items - 1)
  {
    state.end_time = std::chrono::steady_clock::now();
    state.wpm = calculate_wpm(state.correct_chars, state.start_time, state.end_time);
    state.accuracy = (state.total_word_typed == 0) ? 0.0 : (state.correct_words * 100.0) / state.total_word_typed;

    std::vector<std::vector<std::string>> data = {{std::to_string(state.wpm),
                                                   std::to_string(state.accuracy),
                                                   std::to_string(state.correct_words),
                                                   std::to_string(state.total_word_typed)}};

    save_test_data("test_results.csv", data);
  }

  bool is_correct = (state.input == state.items[state.current_index]);
  update_typing_state(state, is_correct);
  screen.Post(Event::Custom);

  return true;
}
