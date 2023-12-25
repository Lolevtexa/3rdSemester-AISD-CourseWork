#pragma once
#include "Libraries.h"
#include "Constants.h"
#include "SandPile.h"

class Camera : public sf::RectangleShape {
private:
    static const sf::Color background;
    static const sf::Color outline;
    static const sf::Color green;
    static const sf::Color purple;
    static const sf::Color gold;
    
    float cellSize;

    SandPile sandPile;

    bool needAddSand = false;
    bool needUpdate = false;

public:
    Camera(sf::Vector2f position, sf::Vector2f size, float cellSize) : cellSize(cellSize) {
        setPosition(position);
        setSize(size);
        setFillColor(background);
    }

    void eventProcessing(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    needAddSand = true;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                needUpdate = true;
            }
        }
    }

    template<typename Func>
    void update(sf::RenderWindow& window, Func func) {
        if (needAddSand) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            if (getGlobalBounds().contains(sf::Vector2f(mousePosition.x, mousePosition.y))) {
                sandPile.addSand((mousePosition.x - getPosition().x) / cellSize, (mousePosition.y - getPosition().y) / cellSize, func());
            }
            needAddSand = false;
        }

        if (needUpdate) {
            sandPile.updateGrid();
            needUpdate = false;
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(static_cast<sf::RectangleShape>(*this), states);

        sf::RectangleShape horizontalLine;
        horizontalLine.setFillColor(outline);
        horizontalLine.setPosition(getPosition());
        horizontalLine.setSize(sf::Vector2f(getSize().x, 2));
        for (int i = 0; i <= getSize().y / cellSize; i++) {
            float x = horizontalLine.getPosition().x;
            float y = cellSize * i - horizontalLine.getSize().y / 2.f;
            horizontalLine.setPosition(x, y);
            target.draw(horizontalLine, states);
        }

        sf::RectangleShape verticalLine;
        verticalLine.setFillColor(outline);
        verticalLine.setPosition(getPosition());
        verticalLine.setSize(sf::Vector2f(2, getSize().y));
        for (int i = 0; i <= getSize().x / cellSize; i++) {
            float x = Constants::TOOL_BAR_WIDTH + cellSize * i - verticalLine.getSize().x / 2.f;
            float y = verticalLine.getPosition().y;
            verticalLine.setPosition(x, y);
            target.draw(verticalLine, states);
        }

        sf::CircleShape sand;
        sand.setRadius(cellSize / 2);
        for (int x = 0; x < Constants::GRID_WIDTH; x++) {
            for (int y = 0; y < Constants::GRID_HEIGHT; y++) {
                if (sandPile.getSandNumber(x, y) > 0) {
                    sand.setPosition(x * cellSize + Constants::TOOL_BAR_WIDTH, y * cellSize);
                    sand.setFillColor(grad(sandPile.getSandNumber(x, y)));
                    target.draw(sand, states);
                }
            }
        }
    }

    void clearGrid() {
        sandPile.clearGrid();
    }

private:
    void saveScreenshot(sf::RenderWindow& window) {
        std::string filename = "../screenshots/";
        filename += std::to_string(std::time(nullptr));
        filename += ".png";

        sf::Texture texture;
        texture.create(window.getSize().x, window.getSize().y);
        texture.update(window);
        texture.copyToImage().saveToFile(filename);
    }

    sf::Color grad(int value) const {
        sf::Color color;
        switch (value) {
        case 0:
            color = background;
            break;

        case 1:
            color = green;
            break;

        case 2:
            color = gold;
            break;

        case 3:
            color = purple;
            break;
        
        default:
            color = outline;
            break;
        }

        return color;
    }
};

const sf::Color Camera::background = sf::Color(255, 255, 255);
const sf::Color Camera::outline = sf::Color(0, 0, 0);
const sf::Color Camera::green = sf::Color(0, 255, 0);
const sf::Color Camera::purple = sf::Color(255, 0, 255);
const sf::Color Camera::gold = sf::Color(255, 215, 0);

