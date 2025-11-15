#include <string>
#include <vector>
#include <chrono>

#include "core/typing_state.hpp"
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

double calculate_wpm(int correct_words,
                     std::chrono::steady_clock::time_point start,
                     std::chrono::steady_clock::time_point end);

std::vector<std::string> get_random_words(int count);

bool handle_space_key(
    TypingState &state,
    int items,
    ScreenInteractive &screen);
