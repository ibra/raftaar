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
  sf::RenderWindow window(sf::VideoMode(1280, 720), "RAFTAAR - STATS DASHBOARD");
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

  double avgWpm = 0.0, maxWpm = 0.0, avgAccuracy = 0.0;
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
      avgWpm = std::accumulate(wpms.begin(), wpms.end(), 0.0) / wpms.size();
      maxWpm = *std::max_element(wpms.begin(), wpms.end());
      avgAccuracy = std::accumulate(accuracies.begin(), accuracies.end(), 0.0) / accuracies.size();
    }
  }

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        window.close();
    }

    window.clear(bg_color);

    // Page title
    auto title = create_text("STATS DASHBOARD", font, 42, main_text, 50, 40);
    title.setStyle(sf::Text::Bold);
    window.draw(title);

    auto subtitle = create_text("Your typing performance overview", font, 18, gray_text, 50, 95);
    window.draw(subtitle);

    auto quit_text = create_text("USE THE ESC KEY TO QUIT", font, 14, gray_text, 50, 750);
    window.draw(quit_text);
    window.display();
  }
}