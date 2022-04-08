#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Inteligencia ");
    sf::Vector2u size;
    sf::Image graph;
    graph.create(size.x, size.y, sf::Color(255, 255, 255));
    // y = 2x
    for (unsigned int x = 0; x < size.x; x++)
    {
        unsigned int y = 2u * x;
        if (y < size.y)
        {
            graph.setPixel(x, y, sf::Color(0, 0, 0));
        }
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(graph);
        window.display();
    }

    return 0;
}