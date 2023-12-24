#pragma once
#include "Libraries.h"
#include "Constants.h"

class Button {
private:
    const int unpressedAlpha = 127;
    const int pressedAlpha = 255;

    std::string filename; 

    sf::RectangleShape button;
    sf::Image icon;
    sf::Texture texture;

    std::function<void()> eventButtonPressed;
public:
    Button(const std::string& filename, int x, int y) {
        this->filename = filename;
        loadTexture(unpressedAlpha);

        button.setSize(sf::Vector2f(Constants::TOOL_BAR_ICON_SIZE, Constants::TOOL_BAR_ICON_SIZE));
        button.setPosition(x, y);
    }

    void eventProcessing(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    loadTexture(pressedAlpha);
                }
            }
        } else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    eventButtonPressed();
                }

                loadTexture(unpressedAlpha);
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

private:
    void loadTexture(int alpha) {
        icon.loadFromFile(filename);
        icon.createMaskFromColor(sf::Color::Black, alpha);
        texture.loadFromImage(icon);
    }
};
