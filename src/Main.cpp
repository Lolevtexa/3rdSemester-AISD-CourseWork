#include "../lib/Libraries.h"
#include "../lib/Constants.h"
#include "../lib/Screen.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), "Sandpile Model");
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    Screen screen(Constants::TOOL_BAR_WIDTH, sf::Vector2f(Constants::WIDTH, Constants::HEIGHT), window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            screen.eventProcessing(event, window);
        }

        screen.update(window);

        window.clear();
        window.draw(screen);
        window.display();
    }

    return 0;
}
