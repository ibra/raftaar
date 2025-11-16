#include "ui/ui_components.hpp"

// this file has custom components we made that render various elements (for the FTXUI lib) which are repeatedly used
// across the different gamemodes.

Element render_stats_box(const Stats &stats)
{
    return vbox({
               text("WPM: " + std::to_string((int)stats.wpm)),
               text("Accuracy: " + std::to_string((int)stats.accuracy) + "%"),
           }) |
           center;
}

Element render_header(const std::string &title)
{
    return text(title) | bold | center;
}

Element render_timer(int remaining_seconds, int warning_threshold)
{
    auto time_color = remaining_seconds <= warning_threshold
                          ? Color::RedLight
                          : Color::GreenLight;

    return hbox({
               text("TIME LEFT: ") | bold,
               text(std::to_string(remaining_seconds) + "s") | color(time_color),
           }) |
           center;
}

Element render_words_box(
    const std::vector<std::string> &items,
    const std::vector<bool> &correctness,
    int current_index,
    int total_items)
{
    std::vector<Element> elements;

    for (int i = 0; i < total_items; i++)
    {
        const auto &item = items[i];
        Element e = text(item);

        if (i < current_index)
        {
            e |= correctness[i] ? color(Color::GreenLight) : color(Color::RedLight);
        }
        else if (i == current_index)
        {
            e |= bold | color(Color::YellowLight);
        }

        elements.push_back(e);
        elements.push_back(text(" "));
    }

    return hflow(std::move(elements)) | border;
}

Element create_default_layout(const LayoutElements &elements, int width)
{
    std::vector<Element> layout;

    layout.push_back(elements.header);
    layout.push_back(separator());

    if (elements.more.get() != nullptr)
    {
        layout.push_back(elements.more);
        layout.push_back(separator());
    }

    layout.push_back(elements.content);
    layout.push_back(separator());
    layout.push_back(elements.stats);
    layout.push_back(separator());
    layout.push_back(elements.input->Render());
    layout.push_back(separator());
    layout.push_back(elements.back_button->Render());

    return vbox(layout) | border | size(WIDTH, EQUAL, width) | center | vcenter;
}

Element create_finished_layout(const Stats &stats, Component back_button)
{
    return vbox({
               text("TEST COMPLETED!") | bold | center,
               separator(),
               text("Final WPM: " + std::to_string((int)stats.wpm)) | bold | center,
               text("Final Accuracy: " + std::to_string((int)stats.accuracy) + "%") | bold | center,
               text("Correct Words: " + std::to_string(stats.correct_words)) | bold | center,
               text("Total Typed: " + std::to_string(stats.total_words)) | bold | center,
               separator(),
               back_button->Render(),
           }) |
           border | size(WIDTH, EQUAL, 120) | center | vcenter;
}

Element render_timer_box(int remaining_time)
{
    return hbox({
               text("TIME: ") | bold,
               text(std::to_string(remaining_time) + "s") |
                   color(remaining_time <= 5 ? Color::RedLight : Color::GreenLight),
           }) |
           center;
}

Element render_timed_words_box(
    const std::vector<std::string> &items,
    const std::vector<bool> &correctness,
    int current_index,
    int visible_start)
{
    std::vector<Element> lines;
    const int WORDS_PER_LINE = 8;
    const int NUM_LINES = 5;
    const int TOTAL_VISIBLE = WORDS_PER_LINE * NUM_LINES;

    for (int line = 0; line < NUM_LINES; line++)
    {
        std::vector<Element> line_elements;
        for (int w = 0; w < WORDS_PER_LINE; w++)
        {
            int idx = visible_start + line * WORDS_PER_LINE + w;

            if (idx >= items.size())
            {
                line_elements.push_back(text(""));
            }
            else
            {
                const auto &word = items[idx];
                Element e = text(word);

                if (idx < current_index)
                {
                    e |= correctness[idx] ? color(Color::GreenLight)
                                          : color(Color::RedLight);
                }
                else if (idx == current_index)
                {
                    e |= bold | color(Color::YellowLight);
                }
                else
                {
                    e |= dim;
                }

                line_elements.push_back(e);
            }

            if (w < WORDS_PER_LINE - 1)
            {
                line_elements.push_back(text(" "));
            }
        }

        lines.push_back(hbox(std::move(line_elements)));
    }

    return vbox(std::move(lines)) | border | size(HEIGHT, EQUAL, 7);
}