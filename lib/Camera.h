#pragma once
#include "Libraries.h"
#include "Sandpile.h"

class Camera : public sf::RectangleShape {
private:
    static const sf::Color background;
    static const sf::Color outline;
    static const sf::Color green;
    static const sf::Color purple;
    static const sf::Color gold;
    
    Sandpile sandpile;

    float cellSize;
    float scale = 1.f;
    sf::Vector2f mousePosition;
    sf::Vector2f position;

    bool needAddSand = false;
    bool updateSandpile = false;
    bool needClearGrid = false;
    bool updatePosition = false;
    bool updateScale = false;
    bool scaleUp = false;
    bool moveTop = false;
    bool moveBottom = false;
    bool moveLeft = false;
    bool moveRight = false;

public:
    Camera(sf::Vector2f position, sf::Vector2f size, float cellSize) : cellSize(cellSize) {
        setPosition(position);
        setSize(size);
        setFillColor(background);
    }

    void eventProcessing(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Right) {
                if (getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    needAddSand = true;
                }
            }
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    updatePosition = true;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                updatePosition = false;
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                updateSandpile = true;
            }
            if (event.key.code == sf::Keyboard::C) {
                needClearGrid = true;
            }
            if (event.key.code == sf::Keyboard::W) {
                moveTop = true;
            }
            if (event.key.code == sf::Keyboard::A) {
                moveLeft = true;
            }
            if (event.key.code == sf::Keyboard::S) {
                moveBottom = true;
            }
            if (event.key.code == sf::Keyboard::D) {
                moveRight = true;
            }
            if (event.key.code == sf::Keyboard::Up) {
                scaleUp = true;
                updateScale = true;
            }
            if (event.key.code == sf::Keyboard::Down) {
                scaleUp = false;
                updateScale = true;
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::W) {
                moveTop = false;
            }
            if (event.key.code == sf::Keyboard::A) {
                moveLeft = false;
            }
            if (event.key.code == sf::Keyboard::S) {
                moveBottom = false;
            }
            if (event.key.code == sf::Keyboard::D) {
                moveRight = false;
            }
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (getGlobalBounds().contains(sf::Vector2f(event.mouseWheelScroll.x, event.mouseWheelScroll.y))) {
                updateScale = true;
                scaleUp = event.mouseWheelScroll.delta > 0;
            }
        }
    }

    template<typename Func>
    void update(sf::RenderWindow& window, Func func) {
        sf::Vector2f globalMousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        
        if (getGlobalBounds().contains(globalMousePosition)) {
            if (updatePosition) {
                position += mousePosition - sf::Vector2f(sf::Mouse::getPosition(window));
            }
            mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        }

        if (needAddSand) {
            if (getGlobalBounds().contains(globalMousePosition)) {
                int x = getGridPosition(globalMousePosition - getHalhCellSize()).x;
                int y = getGridPosition(globalMousePosition - getHalhCellSize()).y;
                sandpile.addSand(x, y, func());
            }
            needAddSand = false;
        }

        if (needClearGrid) {
            sandpile.clearGrid();
            needClearGrid = false;
        }

        if (updateSandpile) {
            sandpile.updateGrid();
            updateSandpile = false;
        }

        if (updateScale) {
            if (scaleUp) {
                scale *= 1.1f;
            } else {
                scale /= 1.1f;
            }
            updateScale = false;
        }

        if (moveTop) {
            position.y -= 10;
        }

        if (moveLeft) {
            position.x -= 10;
        }

        if (moveBottom) {
            position.y += 10;
        }

        if (moveRight) {
            position.x += 10;
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(static_cast<sf::RectangleShape>(*this), states);

        sf::Vector2i topLeft = getGridPosition(getPosition() - getHalhCellSize());
        sf::Vector2i bottomRight = getGridPosition(getPosition() + getSize() + getHalhCellSize());

        sf::RectangleShape horizontalLine;
        horizontalLine.setFillColor(outline);
        horizontalLine.setPosition(getPosition());
        horizontalLine.setSize(sf::Vector2f(getSize().x, 2));
        for (int i = topLeft.y; i <= bottomRight.y; i++) {
            float x = horizontalLine.getPosition().x;
            float y = getCoordinates(0, i).y - horizontalLine.getSize().y / 2.f;
            horizontalLine.setPosition(x, y);
            target.draw(horizontalLine, states);
        }

        sf::RectangleShape verticalLine;
        verticalLine.setFillColor(outline);
        verticalLine.setPosition(getPosition());
        verticalLine.setSize(sf::Vector2f(2, getSize().y));
        for (int i = topLeft.x; i <= bottomRight.x; i++) {
            float x = getCoordinates(i, 0).x - verticalLine.getSize().x / 2.f;
            float y = verticalLine.getPosition().y;
            verticalLine.setPosition(x, y);
            target.draw(verticalLine, states);
        }

        sf::CircleShape sand;
        sand.setRadius(getCellSize() / 2);
        for (int x = topLeft.x; x < bottomRight.x; x++) {
            for (int y = topLeft.y; y < bottomRight.y; y++) {
                if (sandpile.getSandNumber(x, y) > 0) {
                    sand.setPosition(getCoordinates(x, y).x, getCoordinates(x, y).y);
                    sand.setFillColor(grad(sandpile.getSandNumber(x, y)));
                    target.draw(sand, states);
                }
            }
        }
    }

    void clearGrid() {
        sandpile.clearGrid();
    }

    void updateGrid() {
        sandpile.updateGrid();
    }

private:
    float getCellSize() const {
        return cellSize * scale;
    }

    sf::Vector2f getHalhCellSize() const {
        return sf::Vector2f(getCellSize(), getCellSize()) / 2.f;
    }

    sf::Vector2f getDelta() const {
        return getPosition() + getSize() / 2.f - position;
    }
    
    sf::Vector2i getGridPosition(sf::Vector2f _mousePosition) const {
        int x = std::round((_mousePosition - getDelta()).x / getCellSize());
        int y = std::round((_mousePosition - getDelta()).y / getCellSize());
        return sf::Vector2i(x, y);
    }

    sf::Vector2f getCoordinates(int _x, int _y) const {
        float x = (float)_x * getCellSize() + getDelta().x;
        float y = (float)_y * getCellSize() + getDelta().y;
        return sf::Vector2f(x, y);
    }

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
            color = purple;
            break;

        case 3:
            color = gold;
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
