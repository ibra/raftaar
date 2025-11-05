#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <string>
#include <vector>

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

std::vector<std::string> get_random_words(int count) {
  std::vector<std::string> words;
  std::ifstream file("assets/words_alpha.txt");

  std::string word;

  if (!file.is_open())
    return {"error: couldn't open the file."};

  while (file >> word)
    words.push_back(word);

  if (words.empty())
    return {"error: couldnt find any words in the file."};

  std::vector<std::string> result;
  for (int i = 0; i < count; ++i)
    result.push_back(words[rand() % words.size()]);

  return result;
}

void run_words_mode() {
  const int FIXED_WORD_COUNT = 25;

  ScreenInteractive screen = ScreenInteractive::Fullscreen();

  std::string input;
  std::vector<std::string> wordlist = get_random_words(FIXED_WORD_COUNT);

  int currently_typed_word = 0;
  auto start_time = std::chrono::steady_clock::now();

  double wpm = 0.0;
  double accuracy = 100.0;

  auto input_component = Input(&input, "Start typing...");
  auto back_button = Button("Go Back", [&] { screen.ExitLoopClosure()(); });

  auto container = Container::Vertical({
      input_component,
      back_button,
  });

  std::string paragraph_text;
  for (auto &word : wordlist) {
    paragraph_text += word + " ";
  }

  auto renderer = Renderer(container, [&] {
    return vbox({
               text("words mode") | bold | center,
               separator(),
               text(paragraph_text) | border | size(HEIGHT, LESS_THAN, 10),
               separator(),
               input_component->Render(),
               separator(),
               back_button->Render(),
           }) |
           center | border;
  });

  // todo: implement catchevent for space to process words, this is a sample taken from the docs 
  renderer |= CatchEvent([&](Event event) {
    if (event == Event::Character('q')) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  screen.Loop(renderer);
}

int main() {
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  GameMode mode = GameMode::STARTING;
  bool open_stats_page = false;

  srand(time(0));

  if (mode == GameMode::STARTING) {
    auto words_mode_button = Button("WORDS MODE", [&] {
      mode = GameMode::MODE_WORDS;
      run_words_mode();
    });
    auto timed_mode_button = Button("TIMED MODE", [&] {
      mode = GameMode::MODE_WORDS;
      run_words_mode();
    });
    auto equation_mode_button = Button("EQUATION MODE", [&] {
      mode = GameMode::MODE_WORDS;
      run_words_mode();
    });

    auto dashboard_button = Button("OPEN DASHBOARD", [&] {
      mode = GameMode::STATS;
      screen.ExitLoopClosure()();
    });

    auto quit_button = Button("QUIT", [&] {
      mode = GameMode::QUITTING;
      screen.ExitLoopClosure()();
    });

    auto container = Container::Vertical({words_mode_button, timed_mode_button,
                                          equation_mode_button,
                                          dashboard_button, quit_button});

    auto ui = Renderer(container, [&] {
      return vbox({
                 text("RAFTAAR - MAIN MENU"),
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
