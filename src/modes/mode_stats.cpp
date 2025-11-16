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
  sf::RenderWindow window(sf::VideoMode(1280, 720), "RAFTAAR: STATS DASHBOARD", sf::Style::Default);
  window.setFramerateLimit(60);

  // colors used in the stats page
  sf::Color bg_color(20, 20, 25);
  sf::Color card_color(35, 35, 45);
  sf::Color blue_accent(88, 166, 255);
  sf::Color main_text(240, 240, 245);
  sf::Color gray_text(160, 160, 170);

  sf::Font font;
  if (!font.loadFromFile("assets/montserrat.ttf"))
    return;

  auto data = load_test_data("test_results.csv");

  double avg_wpm = 0.0, max_wpm = 0.0, avg_acc = 0.0;
  int totalTests = data.size();

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

  int scroll_offset = 0;
  const int row_height = 70;
  const int max_visible_tests = 3;

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        window.close();

      if (event.type == sf::Event::KeyPressed)
      {
        int max_scroll = std::max(0, (int)data.size() - max_visible_tests);
        if (event.key.code == sf::Keyboard::Up)
          scroll_offset = std::max(0, scroll_offset - 1);
        if (event.key.code == sf::Keyboard::Down)
          scroll_offset = std::min(scroll_offset + 1, max_scroll);
      }
    }

    sf::Vector2u windowSize = window.getSize();
    float contentWidth = std::min(1200.0f, windowSize.x - 100.0f);
    float margin = (windowSize.x - contentWidth) / 2.0f;

    window.clear(bg_color);

    auto title = create_text("RAFTAAR: STATS DASHBOARD", font, 42, main_text, margin, 40);
    title.setStyle(sf::Text::Bold);
    window.draw(title);

    auto subtitle = create_text("An overview of your most recent typing performances.", font, 18, gray_text, margin, 95);
    window.draw(subtitle);

    float cardY = 150;
    float cardHeight = 120;
    float cardWidth = (contentWidth - 40) / 3.0f;
    float spacing = 20;

    auto card1 = create_card(margin, cardY, cardWidth, cardHeight, card_color);
    window.draw(card1);

    auto card1Label = create_text("AVERAGE WPM", font, 16, gray_text, margin + 20, cardY + 30);
    auto card1Value = create_text(format_stat(avg_wpm, 1), font, 48, blue_accent, margin + 20, cardY + 60);
    card1Value.setStyle(sf::Text::Bold);
    window.draw(card1Label);
    window.draw(card1Value);

    auto card2 = create_card(margin + cardWidth + spacing, cardY, cardWidth, cardHeight, card_color);
    window.draw(card2);

    auto card2Label = create_text("BEST WPM", font, 16, gray_text, margin + cardWidth + spacing + 20, cardY + 30);
    auto card2Value = create_text(format_stat(max_wpm, 1), font, 48, blue_accent, margin + cardWidth + spacing + 20, cardY + 60);
    card2Value.setStyle(sf::Text::Bold);
    window.draw(card2Label);
    window.draw(card2Value);

    auto card3 = create_card(margin + (cardWidth + spacing) * 2, cardY, cardWidth, cardHeight, card_color);
    window.draw(card3);

    auto card3Label = create_text("AVG ACCURACY", font, 16, gray_text, margin + (cardWidth + spacing) * 2 + 20, cardY + 30);
    auto card3Value = create_text(format_stat(avg_acc, 1) + "%", font, 48, blue_accent, margin + (cardWidth + spacing) * 2 + 20, cardY + 60);
    card3Value.setStyle(sf::Text::Bold);
    window.draw(card3Label);
    window.draw(card3Value);

    float testsY = cardY + cardHeight + 50;
    auto testsCard = create_card(margin, testsY, contentWidth, 100, card_color);
    window.draw(testsCard);

    auto testsLabel = create_text("TESTS COMPLETED", font, 16, gray_text, margin + 20, testsY + 25);
    auto testsValue = create_text(std::to_string(totalTests), font, 40, main_text, margin + 20, testsY + 45);
    testsValue.setStyle(sf::Text::Bold);
    window.draw(testsLabel);
    window.draw(testsValue);

    float recentY = testsY + 130;
    auto recentTitle = create_text("RECENT TESTS", font, 24, main_text, margin, recentY);
    recentTitle.setStyle(sf::Text::Bold);
    window.draw(recentTitle);

    int displayCount = std::min(max_visible_tests, (int)data.size() - scroll_offset);
    float rowY = recentY + 50;

    for (int i = 0; i < displayCount; ++i)
    {
      int idx = data.size() - 1 - scroll_offset - i;
      if (idx >= 0 && data[idx].size() >= 4)
      {
        auto testCard = create_card(margin, rowY, contentWidth, 60, card_color);
        window.draw(testCard);

        std::string testNum = "#" + std::to_string(data.size() - scroll_offset - i);
        auto numText = create_text(testNum, font, 18, gray_text, margin + 20, rowY + 20);
        window.draw(numText);

        std::string wpmText = format_stat(std::stod(data[idx][0]), 1) + " WPM";
        auto wpm = create_text(wpmText, font, 20, blue_accent, margin + 150, rowY + 18);
        wpm.setStyle(sf::Text::Bold);
        window.draw(wpm);

        std::string accText = format_stat(std::stod(data[idx][1]), 1) + "% ACC";
        auto acc = create_text(accText, font, 18, gray_text, margin + 350, rowY + 20);
        window.draw(acc);

        std::string wordsText = data[idx][2] + "/" + data[idx][3] + " WORDS";
        auto words = create_text(wordsText, font, 18, gray_text, margin + 550, rowY + 20);
        window.draw(words);

        rowY += row_height;
      }
    }

    window.display();
  }
}