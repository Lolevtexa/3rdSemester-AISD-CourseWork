#include "../lib/Libraries.h"
#include "../lib/Button.h"
#include "../lib/ToolBar.h"
#include "../lib/SandPile.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), "Sandpile Model");
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    SandPile sandPile(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            sandPile.eventProcessing(event, window);
        }

        window.clear();
        window.draw(sandPile);
        window.display();
    }

    return 0;
}
