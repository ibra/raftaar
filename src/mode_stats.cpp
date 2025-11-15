#include <SFML/Graphics.hpp>
#include <string>

// this is going to use sfml and have graphs  have to implement
void run_stats_page()
{
  sf::RenderWindow window(sf::VideoMode(1280, 720), "raftaar - stats");

  sf::Font font;
  if (!font.loadFromFile("assets/arial.ttf"))
  {
    printf("error: font file not found\n");
    return;
  }

  sf::Text text("Stats coming soon...", font, 32);
  text.setPosition(100, 100);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    window.draw(text);
    window.display();
  }
}
