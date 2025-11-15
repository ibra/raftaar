#pragma once
#include <chrono>
#include <string>
#include <vector>

struct TypingState
{
    std::string input;
    std::vector<std::string> items;
    std::vector<bool> correctness;

    int current_index = 0;
    int correct_count = 0;
    int total_typed = 0;

    bool started = false;

    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;

    double wpm = 0.0;
    double accuracy = 100.0;
};

void update_typing_state(TypingState &state, bool is_correct);

void start_typing_state(TypingState &state, std::vector<std::string> items);