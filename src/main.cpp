#include "mode_stats.hpp"
#include "mode_types.hpp"
#include "mode_words.hpp"
#include "mode_equations.hpp"
#include "mode_timed.hpp"
#include <cstdlib>
#include <ctime>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

int main() {
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  srand(time(0));

  GameMode mode = GameMode::STARTING;

  // todo: implement timed and equation mode
  auto words_mode_button = Button("WORDS MODE", [&] { run_words_mode(); });
  auto timed_mode_button = Button("EQUATION MODE", [&] { run_equations_mode(); });
  auto equation_mode_button = Button("TIMED MODE", [&] { run_timed_mode(); });

  auto stats_button = Button("OPEN DASHBOARD", [&] { run_stats_page(); });
  auto quit_button = Button("QUIT", [&] { screen.ExitLoopClosure()(); });

  auto modes_container = Container::Horizontal(
      {words_mode_button, timed_mode_button, equation_mode_button});

  auto menu_container = Container::Vertical({stats_button, quit_button});

  auto main_container = Container::Vertical({
      modes_container,
      menu_container,
  });

  auto ui = Renderer(main_container, [&] {
    return vbox({
               text("RAFTAAR - MAIN MENU"),
               separator(),
               modes_container->Render(),
               menu_container->Render(),
           }) |
           center | border;
  });
  screen.Loop(ui);
  return 0;
}
