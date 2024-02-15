#include <SFML/Graphics.hpp>

int main()
{
    // GAME WINDOW
    const std::uint16_t gameWidth = 1280U;
    const std::uint16_t gameHeight = 720U;
    auto window = sf::RenderWindow{{gameWidth, gameHeight}, "GoodGame", sf::Style::Close};

    window.setFramerateLimit(60U);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }

        window.clear(sf::Color(50U, 50U, 50U));


        window.display();
    }
    return 0;
}
