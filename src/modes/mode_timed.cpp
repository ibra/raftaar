#include "utils/mode_utils.hpp"

#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>
#include <chrono>

using namespace ftxui;

std::vector<Element>
create_falling_words(const std::vector<std::string> &wordlist,
                     const std::vector<bool> &word_correctness,
                     int current_index, int visible_start)
{
  std::vector<Element> lines;
  int words_per_line = 8;

  for (int line = 0; line < 5; line++)
  {
    std::vector<Element> line_elements;
    for (int w = 0; w < words_per_line; w++)
    {
      int idx = visible_start + line * words_per_line + w;
      if (idx >= wordlist.size())
      {
        line_elements.push_back(text(""));
      }
      else
      {
        auto &word = wordlist[idx];
        Element e = text(word);
        if (idx < current_index)
        {
          e |= (word_correctness[idx] ? color(Color::GreenLight)
                                      : color(Color::RedLight));
        }
        else if (idx == current_index)
        {
          e |= bold | color(Color::YellowLight);
        }
        else
        {
          e |= dim;
        }
        line_elements.push_back(e);
      }
      if (w < words_per_line - 1)
      {
        line_elements.push_back(text(" "));
      }
    }
    lines.push_back(hbox(std::move(line_elements)));
  }

  return lines;
}

int get_remaining_time(std::chrono::steady_clock::time_point start_time,
                       int duration_seconds)
{
  auto current_time = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time -
                                                                  start_time)
                     .count();
  return duration_seconds - elapsed;
}

void run_timed_mode()
{
  const int DURATION_SECONDS = 15;
  const int WORDS_BATCH_SIZE = 50;

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  std::string input;

  auto wordlist = get_random_words(WORDS_BATCH_SIZE);
  int current_index = 0;
  int visible_start = 0;
  int correct_words = 0;

  int total_typed = 0;
  bool started = false;
  bool finished = false;

  auto start_time = std::chrono::steady_clock::now();
  auto end_time = start_time;

  double wpm = 0.0;
  double accuracy = 100.0;

  int remaining_time = DURATION_SECONDS;

  std::vector<bool> word_correctness;
  word_correctness.reserve(1000);

  for (int i = 0; i < WORDS_BATCH_SIZE; i++)
  {
    word_correctness.push_back(false);
  }

  auto input_component = Input(&input, "TYPE HERE...");
  auto back_button = Button("GO BACK", [&]
                            { screen.ExitLoopClosure()(); });
  auto container = Container::Vertical({input_component, back_button});

  auto renderer = Renderer(container, [&]
                           {
    if (started && !finished) {
      remaining_time = get_remaining_time(start_time, DURATION_SECONDS);
      if (remaining_time <= 0) {
        finished = true;
        remaining_time = 0;
      }
    }

    if (current_index >= visible_start + 30) {
      visible_start = current_index - 10;
    }

    auto falling_lines = create_falling_words(wordlist, word_correctness,
                                              current_index, visible_start);
    auto words_display =
        vbox(std::move(falling_lines)) | border | size(HEIGHT, EQUAL, 7);

    auto timer_box = hbox({
                         text("TIME: ") | bold,
                         text(std::to_string(remaining_time) + "s") |
                             color(remaining_time <= 5 ? Color::RedLight
                                                        : Color::GreenLight),
                     }) |
                     center;

    auto stats_box =
        hbox({
            text("WPM: " + std::to_string((int)wpm)) | border,
            text(" "),
            text("Accuracy: " + std::to_string((int)accuracy) + "%") | border,
            text(" "),
            text("Words: " + std::to_string(correct_words)) | border,
        }) |
        center;

    if (finished) {
      return vbox({
                 text("RAFTAAR - FINISHED!") | bold | center,
                 separator(),
                 words_display,
                 separator(),
                 timer_box,
                 separator(),
                 stats_box,
                 separator(),
                 text("Final WPM: " + std::to_string((int)wpm)) | bold | center,
                 text("Final Accuracy: " + std::to_string((int)accuracy) +
                      "%") |
                     bold | center,
                 separator(),
                 back_button->Render(),
             }) |
             border | size(WIDTH, EQUAL, 120) | center | vcenter;
    }

    return vbox({
               text("TIMED MODE") | bold | center,
               separator(),
               timer_box,
               separator(),
               words_display,
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
    if (finished) {
      return false;
    }

    if (event == Event::Character(' ')) 
    {
      if (!started) {
        started = true;
        start_time = std::chrono::steady_clock::now();
      }

      if (current_index >= wordlist.size() - 20) {
        auto new_words = get_random_words(WORDS_BATCH_SIZE);
        wordlist.insert(wordlist.end(), new_words.begin(), new_words.end());
        for (int i = 0; i < WORDS_BATCH_SIZE; i++) {
          word_correctness.push_back(false);
        }
      }

      total_typed++;
      bool is_correct = (input == wordlist[current_index]);
      word_correctness[current_index] = is_correct;
      if (is_correct)
        correct_words++;
      current_index++;
      input.clear();
      end_time = std::chrono::steady_clock::now();
      wpm = calculate_wpm(correct_words, start_time, end_time);
      accuracy = (double)correct_words / total_typed * 100.0;
      screen.Post(Event::Custom);
      return true;
    } 
    return false; });

  std::thread timer_thread([&screen, &started, &finished]()
                           {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (started && !finished) {
        screen.Post(Event::Custom);
      }
      if (finished) {
        break;
      }
    } });

  screen.Loop(renderer);
  timer_thread.join();
}
