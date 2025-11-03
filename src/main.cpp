#include <SFML/Graphics.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

using namespace ftxui;

void run_stats_page(float value) {
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

    text.setString("value: " + std::to_string(value));
    window.clear();
    window.draw(text);
    window.display();
  }
}

int main() {
  ScreenInteractive screen = ScreenInteractive::TerminalOutput();

  float value = 0.0f;
  bool quit = false;
  bool open_stats_page = false;

  auto increment_button = Button("Increment", [&] { value += 1.0f; });
  auto dashboard_button = Button("Open Dashboard", [&] {
    open_stats_page = true;
    screen.ExitLoopClosure()();
  });
  auto quit_button = Button("Quit", [&] {
    quit = true;
    screen.ExitLoopClosure()();
  });

  auto container =
      Container::Vertical({increment_button, dashboard_button, quit_button});

  auto ui = Renderer(container, [&] {
    return vbox({
               text("raftaar - main menu"),
               separator(),
               text("value: " + std::to_string(value)),
               separator(),
               container->Render(),
           }) |
           center | border;
  });

  screen.Loop(ui);

  if (open_stats_page && !quit)
    run_stats_page(value);

  return 0;
}
