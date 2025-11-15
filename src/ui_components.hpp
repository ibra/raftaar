#pragma once
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

using namespace ftxui;

struct Stats
{
    double wpm;
    double accuracy;
    int correct_count;
    int total_count;
};

Element render_stats_box(const Stats &stats);
Element render_header(const std::string &title);
Element render_timer(int secondsLeft, int warningLimit = 10);

Element render_words_box(
    const std::vector<std::string> &items,
    const std::vector<bool> &correctness,
    int current_index,
    int total_items);