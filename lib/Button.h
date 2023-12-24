#pragma once
#include "Libraries.h"
#include "Constants.h"

class Button : public sf::Drawable{
private:
    static const int unpressedAlpha = 127;
    static const int pressedAlpha = 255;

    std::string filename; 

    sf::RectangleShape button;
    sf::Image icon;
    sf::Texture texture;

    std::function<void()> eventButtonPressed;
public:
    template<typename Func>
    Button(const std::string& filename, int x, int y, Func func) {
        this->filename = filename;
        eventButtonPressed = func;

        button.setSize(sf::Vector2f(Constants::TOOL_BAR_ICON_SIZE, Constants::TOOL_BAR_ICON_SIZE));
        button.setPosition(x, y);

        loadSprite(unpressedAlpha);
    }

    void eventProcessing(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    loadSprite(pressedAlpha);
                }
            }
        } else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    eventButtonPressed();
                }

                loadSprite(unpressedAlpha);
            }
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(button, states);
    }

private:
    void loadSprite(int alpha) {
        if (icon.loadFromFile(filename)) {
            icon.createMaskFromColor(sf::Color::Black, alpha);
            if (texture.loadFromImage(icon)) {
                button.setTexture(&texture);
                return;
            }
        }
    }
};
