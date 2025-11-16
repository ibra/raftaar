#include "utils/mode_utils.hpp"
#include "core/typing_state.hpp"
#include "ui/ui_components.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <thread>

using namespace ftxui;

void run_timed_mode()
{
  const int DURATION_SECONDS = 15;
  const int WORDS_BATCH_SIZE = 50;

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  TypingState state;

  state.is_timed = true;
  state.duration_seconds = DURATION_SECONDS;

  start_typing_state(state, get_random_words(WORDS_BATCH_SIZE));

  bool wants_to_finish = false;

  auto input_component = Input(&state.input, "TYPE HERE...");
  auto back_button = Button("GO BACK", [&]
                            { screen.ExitLoopClosure()(); wants_to_finish = true; });
  auto container = Container::Vertical({input_component, back_button});

  auto renderer = Renderer(container, [&]
                           {
    if (state.started && !state.finished) {
      state.remaining_time = get_remaining_time(state.start_time, DURATION_SECONDS);
      if (state.remaining_time <= 0) {
        state.finished = true;
        state.remaining_time = 0;
      }
    }
    
    if (state.current_index >= state.visible_start + 8) {
      state.visible_start = state.current_index - 4;
    }
    
    if (state.current_index >= state.items.size() - 16) {
      auto new_words = get_random_words(WORDS_BATCH_SIZE);
      state.items.insert(state.items.end(), new_words.begin(), new_words.end());
      state.correctness.resize(state.items.size(), false);
    }
    
    if (state.finished) {
      Stats stats = {state.wpm, state.accuracy, state.correct_words, state.total_words};
      return create_finished_layout(stats, back_button);
    }
    
    auto content = render_timed_words_box(
        state.items,
        state.correctness,
        state.current_index,
        state.visible_start
    );
    
    Stats stats = {state.wpm, state.accuracy, state.correct_words, state.total_words};
    
    LayoutElements layout;
    layout.content = content;
    layout.stats = render_stats_box(stats);
    layout.header = render_header("TIMED MODE");
    layout.more = render_timer_box(state.remaining_time);
    layout.input = input_component;
    layout.back_button = back_button;
    
    return create_default_layout(layout); });

  renderer |= CatchEvent([&](Event event)
                         {
    if (state.finished) {
      return false;
    }
    
    if (event == Event::Character(' ')) {
      if (!state.started) {
        state.started = true;
        state.start_time = std::chrono::steady_clock::now();
      }
      
      bool is_correct = (state.input == state.items[state.current_index]);
      update_typing_state(state, is_correct);
      screen.Post(Event::Custom);
      return true;
    }
    return false; });

  std::thread timer_thread([&screen, &state, &wants_to_finish]()
                           {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (state.started && !state.finished && !wants_to_finish) {
        screen.Post(Event::Custom);
      }
      if (state.finished || wants_to_finish) {
        break;
      }
    } });

  screen.Loop(renderer);
  timer_thread.join();
}