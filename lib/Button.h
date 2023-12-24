#pragma once
#include "Libraries.h"
#include "Constants.h"

class Button {
private:
    sf::RectangleShape button;
    sf::Image icon;
    sf::Texture texture;

    std::function<void()> eventButtonPressed;
public:
    Button(const std::string& filename, int x, int y) {
        icon.loadFromFile(filename);
        texture.loadFromImage(icon);

        button.setSize(sf::Vector2f(Constants::TOOL_BAR_ICON_SIZE, Constants::TOOL_BAR_ICON_SIZE));
        button.setPosition(x, y);
    }

    void eventProcessing(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    eventButtonPressed();
                }
            }
        }
    }

    template<typename Func>
    void setEventButtonPressed(Func func) {
        eventButtonPressed = func;
    }

    void draw(sf::RenderWindow& window) {
        button.setTexture(&texture);
        window.draw(button);
    }
};
