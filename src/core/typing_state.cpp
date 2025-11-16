#include "core/typing_state.hpp"
#include "utils/mode_utils.hpp"

// updates the typing state after each input

void update_typing_state(TypingState &state, bool is_correct)
{
    if (!state.started)
    {
        state.started = true;
        state.start_time = std::chrono::steady_clock::now();
    }

    state.total_words++;
    state.correctness[state.current_index] = is_correct;

    if (is_correct)
    {
        state.correct_words++;
        state.correct_chars += state.items[state.current_index].length() + 1; // the +1 here is added to also account for the space
    }

    state.current_index++;
    state.input.clear();
    state.end_time = std::chrono::steady_clock::now();

    state.wpm = calculate_wpm(state.correct_chars, state.start_time, state.end_time);
    state.accuracy = (double)state.correct_words / state.total_words * 100.0;
}

// when any mode begins we initialize the typing state by setting all of its values to default

void start_typing_state(TypingState &state, std::vector<std::string> items)
{
    state.items = items;
    state.correctness.resize(state.items.size(), false);

    state.started = false;
    state.finished = false;

    state.current_index = 0;

    state.total_words = 0;
    state.correct_words = 0;

    state.correct_chars = 0;

    state.wpm = 0.0;
    state.accuracy = 100.0;
}