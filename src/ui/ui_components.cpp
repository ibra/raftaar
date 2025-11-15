#include "ui/ui_components.hpp"

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