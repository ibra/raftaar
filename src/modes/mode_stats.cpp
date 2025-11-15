#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>

#include "data/data_handler.hpp"
#include "utils/mode_utils.hpp"

// function to quickly make cards instead of repeating the same code
sf::RectangleShape create_card(float x, float y, float width, float height, sf::Color fillColor)
{
  sf::RectangleShape card(sf::Vector2f(width, height));
  card.setPosition(x, y);
  card.setFillColor(fillColor);
  return card;
}

// this is also a helper function to be able to quickly make all the text data
sf::Text create_text(const std::string &str, const sf::Font &font, int size, sf::Color color, float x, float y)
{
  sf::Text text;
  text.setFont(font);
  text.setString(str);
  text.setCharacterSize(size);
  text.setFillColor(color);
  text.setPosition(x, y);
  return text;
}

// to display all the stats to 2 d.p.
std::string format_stat(double value, int precision = 2)
{
  std::ostringstream string;
  string << std::fixed << std::setprecision(precision) << value;
  return string.str();
}

void run_stats_page()
{
  sf::RenderWindow window(sf::VideoMode(1280, 720), "RAFTAAR: STATS DASHBOARD");
  window.setFramerateLimit(60);

  // colors used in the stats page

  sf::Color bg_color(20, 20, 25);
  sf::Color card_color(35, 35, 45);
  sf::Color blue_accent(88, 166, 255);
  sf::Color main_text(240, 240, 245);
  sf::Color gray_text(160, 160, 170);

  sf::Font font;
  if (!font.loadFromFile("assets/arial.ttf"))
    return;

  // load the test data from the csv file

  auto data = load_test_data("test_results.csv");

  double avg_wpm = 0.0, max_wpm = 0.0, avg_acc = 0.0;
  int totalTests = data.size();

  // its possible that the user hasn't done any tests yet

  if (!data.empty())
  {
    std::vector<double> wpms, accuracies;
    for (const auto &row : data)
    {
      if (row.size() >= 4)
      {
        wpms.push_back(std::stod(row[0]));
        accuracies.push_back(std::stod(row[1]));
      }
    }

    if (!wpms.empty())
    {
      avg_wpm = std::accumulate(wpms.begin(), wpms.end(), 0.0) / wpms.size();
      max_wpm = *std::max_element(wpms.begin(), wpms.end());
      avg_acc = std::accumulate(accuracies.begin(), accuracies.end(), 0.0) / accuracies.size();
    }
  }

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();

      // let closing the stats page also be possible with escape key
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        window.close();
    }

    window.clear(bg_color);

    auto title = create_text("RAFTAAR: STATS DASHBOARD", font, 42, main_text, 50, 40);
    title.setStyle(sf::Text::Bold);
    window.draw(title);

    // todo: in the future i want to write a custom scrollable component so i can support scrolling for
    // all data, but for now we show the most recent 5 tests.

    auto subtitle = create_text("An overview of your most recent typing performances.", font, 18, gray_text, 50, 95);
    window.draw(subtitle);

    // main stats card section

    float cardY = 150;
    float cardHeight = 120;
    float cardWidth = 280;
    float spacing = 20;

    // average wpm gui
    auto card1 = create_card(50, cardY, cardWidth, cardHeight, card_color);
    window.draw(card1);

    auto card1Label = create_text("AVERAGE WPM", font, 16, gray_text, 70, cardY + 30);
    auto card1Value = create_text(format_stat(avg_wpm, 1), font, 48, blue_accent, 70, cardY + 60);
    card1Value.setStyle(sf::Text::Bold);
    window.draw(card1Label);
    window.draw(card1Value);

    // best wpm gui

    auto card2 = create_card(50 + cardWidth + spacing, cardY, cardWidth, cardHeight, card_color);
    window.draw(card2);

    auto card2Label = create_text("BEST WPM", font, 16, gray_text, 70 + cardWidth + spacing, cardY + 30);
    auto card2Value = create_text(format_stat(max_wpm, 1), font, 48, blue_accent, 70 + cardWidth + spacing, cardY + 60);
    card2Value.setStyle(sf::Text::Bold);
    window.draw(card2Label);
    window.draw(card2Value);

    // accuracy gui
    auto card3 = create_card(50 + (cardWidth + spacing) * 2, cardY, cardWidth, cardHeight, card_color);
    window.draw(card3);

    auto card3Label = create_text("AVG ACCURACY", font, 16, gray_text, 70 + (cardWidth + spacing) * 2, cardY + 30);
    auto card3Value = create_text(format_stat(avg_acc, 1) + "%", font, 48, blue_accent, 70 + (cardWidth + spacing) * 2, cardY + 60);
    card3Value.setStyle(sf::Text::Bold);
    window.draw(card3Label);
    window.draw(card3Value);

    // show all the tests completed

    float testsY = cardY + cardHeight + 50;
    auto testsCard = create_card(50, testsY, 900, 100, card_color);
    window.draw(testsCard);

    auto testsLabel = create_text("TESTS COMPLETED", font, 16, gray_text, 70, testsY + 25);
    auto testsValue = create_text(std::to_string(totalTests), font, 40, main_text, 70, testsY + 45);

    testsValue.setStyle(sf::Text::Bold);

    window.draw(testsLabel);
    window.draw(testsValue);

    // recent tests list

    float recentY = testsY + 130;
    auto recentTitle = create_text("RECENT TESTS", font, 24, main_text, 50, recentY);
    recentTitle.setStyle(sf::Text::Bold);
    window.draw(recentTitle);

    // since tests could also be less than five and that owuld cause an error when indexing

    int displayCount = std::min(5, (int)data.size());
    float rowY = recentY + 50;

    for (int i = 0; i < displayCount; ++i)
    {
      int idx = data.size() - 1 - i;
      if (data[idx].size() >= 4)
      {
        auto testCard = create_card(50, rowY, 900, 60, card_color);
        window.draw(testCard);

        std::string testNum = "#" + std::to_string(data.size() - i);
        auto numText = create_text(testNum, font, 18, gray_text, 70, rowY + 20);
        window.draw(numText);

        std::string wpmText = format_stat(std::stod(data[idx][0]), 1) + " WPM";
        auto wpm = create_text(wpmText, font, 20, blue_accent, 250, rowY + 18);
        wpm.setStyle(sf::Text::Bold);
        window.draw(wpm);

        std::string accText = format_stat(std::stod(data[idx][1]), 1) + "% ACCURACY";
        auto acc = create_text(accText, font, 18, gray_text, 450, rowY + 20);
        window.draw(acc);

        std::string wordsText = data[idx][2] + "/" + data[idx][3] + " WORDS";
        auto words = create_text(wordsText, font, 18, gray_text, 700, rowY + 20);
        window.draw(words);

        rowY += 100;
      }
    }

    auto quit_text = create_text("USE THE ESC KEY TO QUIT", font, 14, gray_text, 50, 750);
    window.draw(quit_text);
    window.display();
  }
}