#include "mode_utils.hpp"
#include <cstdlib>
#include <ctime>
#include <equation_generation.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>
using namespace ftxui;

void run_equations_mode()
{
  const int FIXED_EQUATION_COUNT = 10;

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  std::string input;
  auto equationlist = generate_equations(FIXED_EQUATION_COUNT);

  int current_index = 0;
  int correct_equations = 0;
  int total_typed = 0;

  bool started = false;
  auto start_time = std::chrono::steady_clock::now();
  auto end_time = start_time;

  double wpm = 0.0;
  double accuracy = 100.0;

  std::vector<bool> equation_correctness(FIXED_EQUATION_COUNT, false);

  auto input_component = Input(&input, "TYPE HERE...");
  auto back_button = Button("GO BACK", [&]
                            { screen.ExitLoopClosure()(); });

  auto container = Container::Vertical({input_component, back_button});

  auto renderer = Renderer(container, [&]
                           {
    std::vector<Element> equation_elements;
    for (int i = 0; i < FIXED_EQUATION_COUNT; i++) {
      auto &equation = equationlist[i];
      Element e = text(equation);
      if (i < current_index) {
        e |= (equation_correctness[i] ? color(Color::GreenLight)
                                      : color(Color::RedLight));
      } else if (i == current_index) {
        e |= bold | color(Color::YellowLight);
      }
      equation_elements.push_back(e);
      equation_elements.push_back(text(" "));
    }

    auto paragraph = hflow(std::move(equation_elements)) | border;
    auto stats_box =
        vbox({
            text("WPM: " + std::to_string((int)wpm)),
            text("Accuracy: " + std::to_string((int)accuracy) + "%"),
        }) |
        center;
    return vbox({
               text("EQUATIONS MODE") | bold | center,
               separator(),
               paragraph,
               separator(),
               stats_box,
               separator(),
               input_component->Render(),
               separator(),
               back_button->Render(),
           }) |
           border | size(WIDTH, EQUAL, 120) | center | vcenter; });

  renderer |= CatchEvent([&](Event event)
                         {
    if (event == Event::Character(' ')) {
      if (!started) {
        started = true;
        start_time = std::chrono::steady_clock::now();
      }
      if (current_index < FIXED_EQUATION_COUNT) {
        total_typed++;
        bool is_correct = (input == equationlist[current_index]);
        equation_correctness[current_index] = is_correct;
        if (is_correct)
          correct_equations++;
        current_index++;
        input.clear();
        end_time = std::chrono::steady_clock::now();
        wpm = calculate_wpm(correct_equations, start_time, end_time);
        accuracy = (double)correct_equations / total_typed * 100.0;
        screen.Post(Event::Custom);
      }
      return true;
    }
    return false; });

  screen.Loop(renderer);
}
