#include "utils/mode_utils.hpp"
#include "core/typing_state.hpp"
#include "ui/ui_components.hpp"
#include "utils/equation_generation.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

void run_equations_mode()
{
    const int FIXED_EQUATION_COUNT = 10;

    ScreenInteractive screen = ScreenInteractive::Fullscreen();
    std::string input;

    TypingState state;
    start_typing_state(state, generate_equations(FIXED_EQUATION_COUNT));

    auto input_component = Input(&state.input, "TYPE HERE...");
    auto back_button = Button("GO BACK", [&]
                              { screen.ExitLoopClosure()(); });
    auto container = Container::Vertical({input_component, back_button});

    auto renderer = Renderer(container, [&]
                             {
      auto content = render_words_box(
          state.items,
          state.correctness,
          state.current_index,
          FIXED_EQUATION_COUNT
      );
      
      Stats stats = {state.wpm, state.accuracy, state.correct_words, state.total_words};
      
      LayoutElements layout;
      layout.content = content;
      layout.stats = render_stats_box(stats);
      layout.header = render_header("EQUATION MODE");
      layout.more = text("");
      layout.input = input_component;
      layout.back_button = back_button;
      
      return create_default_layout(layout); });

    renderer |= CatchEvent([&](Event event)
                           {
      if (event == Event::Character(' ')) {
          return handle_space_key(state, FIXED_EQUATION_COUNT, screen);
      }
      return false; });

    screen.Loop(renderer);
}
