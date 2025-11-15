#include <SFML/Graphics.hpp>
#include "utils/mode_utils.hpp"
#include <string>

void run_stats_page()
{
  sf::RenderWindow window(sf::VideoMode(1000, 800), "RAFTAAR - STATS DASHBOARD");
  window.setFramerateLimit(60);

  sf::Font font;
  font.loadFromFile("assets/arial.ttf");

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color(30, 30, 30));
    window.display();
  }
}
