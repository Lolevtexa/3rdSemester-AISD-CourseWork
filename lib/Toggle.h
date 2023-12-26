#pragma once
#include "Libraries.h"
#include "Constants.h"

class Toggle : 
    public sf::Drawable {
private:
    static const int unpressedAlpha = 127;
    static const int pressedAlpha = 255;

    std::string filename;
    bool isActive = false;
    bool updateSprite = false;

    sf::RectangleShape toggle;
    sf::Image icon;
    sf::Texture texture;

    std::function<void()> eventToggleChanged;
    
public:
    template<typename Func>
    Toggle(const std::string& filename, int x, int y, Func func) {
        this->filename = filename;
        eventToggleChanged = func;

        toggle.setSize(sf::Vector2f(Constants::TOOL_BAR_ICON_SIZE, Constants::TOOL_BAR_ICON_SIZE));
        toggle.setPosition(x, y);

        loadSprite(unpressedAlpha);
    }

    Toggle(const Toggle & other) :
            filename(other.filename),
            eventToggleChanged(other.eventToggleChanged),
            toggle(other.toggle),
            icon(other.icon),
            texture(other.texture) {
        toggle.setTexture(&texture);
    }

    void eventProcessing(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (toggle.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    isActive = !isActive;
                    updateSprite = true;
                }
            }
        }
    }

    void update() {
        if (isActive) {
            eventToggleChanged();
            updateSprite = true;
        }

        if (updateSprite) {
            if (isActive) {
                loadSprite(pressedAlpha);
            } else {
                loadSprite(unpressedAlpha);
            }
            updateSprite = false;
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(toggle, states);
    }

private:
    void loadSprite(int alpha) {
        if (icon.loadFromFile(filename)) {
            icon.createMaskFromColor(sf::Color::Black, alpha);
            if (texture.loadFromImage(icon)) {
                toggle.setTexture(&texture);
                return;
            }
        }
    }
};
