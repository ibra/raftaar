#include "mode_utils.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

using namespace ftxui;

void run_words_mode() {
  const int FIXED_WORD_COUNT = 50;
  ScreenInteractive screen = ScreenInteractive::Fullscreen();

  std::string input;
  auto wordlist = get_random_words(FIXED_WORD_COUNT);

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
    for (int i = 0; i < FIXED_WORD_COUNT; i++) {
      auto &word = wordlist[i];
      Element e = text(word);
      if (i < current_index)
        e |= (i < correct_words ? color(Color::GreenLight)
                                : color(Color::RedLight));
      else if (i == current_index)
        e |= bold | color(Color::YellowLight);
      word_elements.push_back(e);
      word_elements.push_back(text(" "));
    }

    auto paragraph = hflow(std::move(word_elements)) | border;

    auto stats_box =
        vbox({
            text("WPM: " + std::to_string((int)wpm)),
            text("Accuracy: " + std::to_string((int)accuracy) + "%"),
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
        if (input == wordlist[current_index])
          correct_words++;
        current_index++;
        input.clear();
        end_time = std::chrono::steady_clock::now();
        wpm = calculate_wpm(correct_words, start_time, end_time);
        accuracy = (double)correct_words / total_typed * 100.0;
        screen.Post(Event::Custom);
      }
      return true;
    }
    return false;
  });

  screen.Loop(renderer);
}
