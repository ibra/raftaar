#include <SFML/Graphics.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

using namespace ftxui;

enum GameMode {
  STARTING,
  MODE_WORDS,
  MODE_TIMED,
  MODE_EQUATION,
  STATS,
  QUITTING
};

void run_stats_page() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "raftaar - stats");

  sf::Font font;
  if (!font.loadFromFile("assets/arial.ttf")) {
    printf("error: font file not found\n");
    return;
  }

  sf::Text text("", font, 32);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    window.draw(text);
    window.display();
  }
}

void run_typing_test(GameMode mode) {}

int main() {
  ScreenInteractive screen = ScreenInteractive::TerminalOutput();
  GameMode mode = GameMode::STARTING;
  bool open_stats_page = false;

  if (mode == GameMode::STARTING) {
    auto words_mode_button =
        Button("Words Mode", [&] { run_typing_test(GameMode::MODE_WORDS); });
    auto timed_mode_button =
        Button("Timed Mode", [&] { run_typing_test(GameMode::MODE_TIMED); });
    auto equation_mode_button = Button(
        "Equation Mode", [&] { run_typing_test(GameMode::MODE_EQUATION); });

    auto dashboard_button = Button("Open Dashboard", [&] {
      open_stats_page = true;
      screen.ExitLoopClosure()();
    });

    auto quit_button = Button("Quit", [&] {
      mode = GameMode::QUITTING;
      screen.ExitLoopClosure()();
    });

    auto container = Container::Vertical(
        {words_mode_button, timed_mode_button, equation_mode_button});

    auto ui = Renderer(container, [&] {
      return vbox({
                 text("raftaar - main menu"),
                 separator(),
                 separator(),
                 container->Render(),
             }) |
             center | border;
    });

    screen.Loop(ui);
  } else if (mode == GameMode::STATS) {
    run_stats_page();
  }

    return 0;
}
