#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include <vector>

using namespace ftxui;

struct Stats
{
    double wpm;
    double accuracy;
    int correct_words;
    int total_words;
};

Element render_stats_box(const Stats &stats);
Element render_header(const std::string &title);
Element render_timer(int secondsLeft, int warningLimit = 10);

Element render_words_box(
    const std::vector<std::string> &items,
    const std::vector<bool> &correctness,
    int current_index,
    int total_items);

struct LayoutElements
{
    Element content;
    Element stats;
    Element header;
    Element more; // todo: use this for the timer later
    Component input;
    Component back_button;
};

Element create_default_layout(const LayoutElements &elements, int width = 120);

Element create_finished_layout(const Stats &stats, Component back_button); // ADD THIS