#include "core/typing_state.hpp"
#include "utils/mode_utils.hpp"

void update_typing_state(TypingState &state, bool is_correct)
{
    if (!state.started)
    {
        state.started = true;
        state.start_time = std::chrono::steady_clock::now();
    }

    state.total_typed++;
    state.correctness[state.current_index] = is_correct;

    if (is_correct)
    {
        state.correct_count++;
    }

    state.current_index++;
    state.input.clear();
    state.end_time = std::chrono::steady_clock::now();

    state.wpm = calculate_wpm(state.correct_count, state.start_time, state.end_time);
    state.accuracy = (double)state.correct_count / state.total_typed * 100.0;
}

void start_typing_state(TypingState &state, std::vector<std::string> items)
{
    state.items = items;
    state.correctness.resize(state.items.size(), false);

    state.current_index = 0;
    state.correct_count = 0;
    state.total_typed = 0;

    state.started = false;
    state.wpm = 0.0;
    state.accuracy = 100.0;
}