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
    bool finished = false;

    int total_words = 0;
    int correct_words = 0;
    int correct_chars = 0;

    bool started = false;

    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;

    double wpm = 0.0;
    double accuracy = 100.0;

    bool is_timed = false;
    int duration_seconds = 0;
    int remaining_time = 0;
    int visible_start = 0;
    std::chrono::steady_clock::time_point start_time;
};

void update_typing_state(TypingState &state, bool is_correct);

void start_typing_state(TypingState &state, std::vector<std::string> items);