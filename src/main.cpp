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

double calculate_wpm(int words_typed,
                     std::chrono::steady_clock::time_point start,
                     std::chrono::steady_clock::time_point end) {
  double elapsed_seconds =
      duration_cast<std::chrono::duration<double>>(end - start).count();
  double elapsed_minutes = elapsed_seconds / 60.0;

  if (elapsed_minutes == 0) {
    return 0.0;
  }

  return words_typed / elapsed_minutes;
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
  const int FIXED_WORD_COUNT = 5;
  ScreenInteractive screen = ScreenInteractive::Fullscreen();

  std::string input;
  std::vector<std::string> wordlist = get_random_words(FIXED_WORD_COUNT);

  int current_index = 0;
  int correct_words = 0;
  int total_typed = 0;

  bool started = false;
  auto start_time = std::chrono::steady_clock::now();
  auto end_time = start_time;

  double wpm = 0.0;
  double accuracy = 100.0;

  auto input_component = Input(&input, "TYPE HERE...");
  auto back_button = Button("GO BACK", [&] { screen.ExitLoopClosure()(); });
  auto container = Container::Vertical({input_component, back_button});

  auto renderer = Renderer(container, [&] {
    std::vector<Element> word_elements;

    // todo: this works, but many words dont wrap for some reason
    for (int i = 0; i < FIXED_WORD_COUNT; i++) {
      std::string &word = wordlist[i];
      Element e = text(word);

      if (i < current_index) {
        e |= (i < correct_words ? color(Color::GreenLight)
                                : color(Color::RedLight));
      } else if (i == current_index) {
        e |= bold | color(Color::YellowLight);
      }

      word_elements.push_back(e);
      word_elements.push_back(text(" "));
    }

    auto paragraph = hbox(std::move(word_elements)) | border;

    auto stats_box =
        vbox({
            text("WPM: " + std::to_string(static_cast<int>(wpm))),
            text("Accuracy: " + std::to_string(static_cast<int>(accuracy)) +
                 "%"),
        }) |
        center;

    return vbox({
               text("WORDS MODE") | bold | center,
               separator(),
               paragraph,
               separator(),
               stats_box,
               separator(),
               input_component->Render(),
               separator(),
               back_button->Render(),
           }) |
           border | size(WIDTH, EQUAL, 120) | center | vcenter;
  });

  renderer |= CatchEvent([&](Event event) {
    if (event == Event::Character(' ')) {
      if (!started) {
        started = true;
        start_time = std::chrono::steady_clock::now();
      }

      if (current_index < FIXED_WORD_COUNT) {
        total_typed++;

        std::string expected = wordlist[current_index];
        std::string typed = input;

        if (typed == expected)
          correct_words++;

        current_index++;
        input.clear();

        end_time = std::chrono::steady_clock::now();
        wpm = calculate_wpm(correct_words, start_time, end_time);
        accuracy = total_typed > 0 ? (double)correct_words / total_typed * 100.0
                                   : 100.0;

        screen.Post(Event::Custom); // refresh the screen after event triggered
      }
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

    auto modes_container = Container::Horizontal(
        {words_mode_button, timed_mode_button, equation_mode_button});
    auto options_container =
        Container::Vertical({dashboard_button, quit_button});
    auto overall_container =
        Container::Vertical({modes_container, options_container});

    auto ui = Renderer(overall_container, [&] {
      return vbox({
                 text("RAFTAAR - MAIN MENU"),
                 separator(),
                 separator(),
                 overall_container->Render(),
             }) |
             center | border;
    });

    screen.Loop(ui);
  } else if (mode == GameMode::STATS) {
    run_stats_page();
  }

  return 0;
}
