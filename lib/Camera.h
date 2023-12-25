#pragma once
#include "Libraries.h"
#include "SandPile.h"

class Camera : public sf::RectangleShape {
private:
    static const sf::Color background;
    static const sf::Color outline;
    static const sf::Color green;
    static const sf::Color purple;
    static const sf::Color gold;
    
    SandPile sandPile;

    float cellSize;
    float scale = 1.f;
    sf::Vector2f mousePosition;

    bool needAddSand = false;
    bool needUpdate = false;

public:
    Camera(sf::Vector2f position, sf::Vector2f size, float cellSize) : cellSize(cellSize) {
        setPosition(position);
        setSize(size);
        setFillColor(background);
    }

    void updateMousePosition(sf::RenderWindow& window) {
        if (getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
            mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        }
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

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (getGlobalBounds().contains(sf::Vector2f(event.mouseWheelScroll.x, event.mouseWheelScroll.y))) {
                if (event.mouseWheelScroll.delta > 0) {
                    scale *= 1.1f;
                }
                else {
                    scale /= 1.1f;
                }
            }
        }
    }

    template<typename Func>
    void update(sf::RenderWindow& window, Func func) {
        if (needAddSand) {
            sf::Vector2f globalMousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
            if (getGlobalBounds().contains(globalMousePosition)) {
                int x = getGridPosition(sf::Vector2f(globalMousePosition.x - getCellSize() / 2.f, globalMousePosition.y - getCellSize() / 2.f)).x;
                int y = getGridPosition(sf::Vector2f(globalMousePosition.x - getCellSize() / 2.f, globalMousePosition.y - getCellSize() / 2.f)).y;
                sandPile.addSand(x, y, func());
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
        for (int i = -getSizeScale().y / 2.f; i <= getSizeScale().y / 2.f; i++) {
            float x = horizontalLine.getPosition().x;
            float y = getCoordinates(0, i).y - horizontalLine.getSize().y / 2.f;
            horizontalLine.setPosition(x, y);
            target.draw(horizontalLine, states);
        }

        sf::RectangleShape verticalLine;
        verticalLine.setFillColor(outline);
        verticalLine.setPosition(getPosition());
        verticalLine.setSize(sf::Vector2f(2, getSize().y));
        for (int i = -getSizeScale().x / 2.f; i <= getSizeScale().x / 2.f; i++) {
            float x = getCoordinates(i, 0).x - verticalLine.getSize().x / 2.f;
            float y = verticalLine.getPosition().y;
            verticalLine.setPosition(x, y);
            target.draw(verticalLine, states);
        }

        sf::CircleShape sand;
        sand.setRadius(getCellSize() / 2);
        for (int x = -getSizeScale().x / 2.f; x < getSizeScale().x / 2.f; x++) {
            for (int y = -getSizeScale().y / 2.f; y < getSizeScale().y / 2.f; y++) {
                if (sandPile.getSandNumber(x, y) > 0) {
                    sand.setPosition(getCoordinates(x, y).x, getCoordinates(x, y).y);
                    sand.setFillColor(grad(sandPile.getSandNumber(x, y)));
                    target.draw(sand, states);
                }
            }
        }
    }

    void clearGrid() {
        sandPile.clearGrid();
    }

    void updateGrid() {
        sandPile.updateGrid();
    }

private:
    float getCellSize() const {
        return cellSize * scale;
    }

    sf::Vector2f getSizeScale() const {
        float x = getSize().x / getCellSize();
        float y = getSize().y / getCellSize();
        return sf::Vector2f(x, y);
    }
    
    int round_int( float r ) const {
        return (r > 0.0) ? (r + 0.5) : (r - 0.5); 
    }
    
    sf::Vector2i getGridPosition(sf::Vector2f _mousePosition) const {
        int x = round_int(((_mousePosition.x - (getPosition().x + getSize().x / 2.f)) / scale) / cellSize);
        int y = round_int(((_mousePosition.y - (getPosition().y + getSize().y / 2.f)) / scale) / cellSize);
        return sf::Vector2i(x, y);
    }

    sf::Vector2f getCoordinates(int _x, int _y) const {
        float x = ((float)_x * cellSize) * scale + (getPosition().x + getSize().x / 2.f);
        float y = ((float)_y * cellSize) * scale + (getPosition().y + getSize().y / 2.f);
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
