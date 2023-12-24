#pragma once
#include "Libraries.h"
#include "Constants.h"
#include "ToolBar.h"
#include "Button.h"

class SandPile : public sf::Drawable{
private:
    static const sf::Color background;
    static const sf::Color outline;
    static const sf::Color green;
    static const sf::Color purple;
    static const sf::Color gold;

    ToolBar toolBar;

    std::vector<std::vector<int>> grid;

public:
    SandPile(sf::RenderWindow& window) {
        toolBar.addButton("../assets/img/screenshot.png", [this, &window]() {
            saveScreenshot(window);
        });

        toolBar.addButton("../assets/img/clear.png", [this]() {
            clearGrid();
        });

        grid.resize(Constants::GRID_WIDTH, std::vector<int>(Constants::GRID_HEIGHT, 0));
    }

    void eventProcessing(sf::Event& event, sf::RenderWindow& window) {
        toolBar.eventProcessing(event);

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (event.mouseButton.x >= Constants::TOOL_BAR_WIDTH) {
                    addSand((event.mouseButton.x - Constants::TOOL_BAR_WIDTH) / Constants::CELL_SIZE, event.mouseButton.y / Constants::CELL_SIZE);
                }
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                updateGrid();
            }

            if (event.key.code == sf::Keyboard::S) {
                saveScreenshot(window);
            }
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(toolBar, states);

        for (int x = 0; x < Constants::GRID_WIDTH; x++) {
            for (int y = 0; y < Constants::GRID_HEIGHT; y++) {
                sf::RectangleShape cell(sf::Vector2f(Constants::CELL_SIZE, Constants::CELL_SIZE));
                cell.setPosition(x * Constants::CELL_SIZE + Constants::TOOL_BAR_WIDTH, y * Constants::CELL_SIZE);
                cell.setFillColor(background);
                cell.setOutlineColor(outline);
                cell.setOutlineThickness(-0.5f);
                target.draw(cell, states);

                if (grid[x][y] > 0) {
                    sf::CircleShape sand(Constants::CELL_SIZE / 2);
                    sand.setPosition(x * Constants::CELL_SIZE + Constants::TOOL_BAR_WIDTH, y * Constants::CELL_SIZE);
                    sand.setFillColor(grad(grid[x][y]));
                    target.draw(sand, states);
                }
            }
        }
    }

private:
    void addSand(int x, int y) {
        grid[x][y]++;
    }

    void topple(int x, int y) {
        grid[x][y] -= 4;
        if (y - 1 >= 0) grid[x][y - 1]++;
        if (y + 1 < Constants::GRID_HEIGHT) grid[x][y + 1]++;
        if (x - 1 >= 0) grid[x - 1][y]++;
        if (x + 1 < Constants::GRID_WIDTH) grid[x + 1][y]++;
    }

    void toppleAll() {
        for (int x = 0; x < Constants::GRID_WIDTH; x++) {
            for (int y = 0; y < Constants::GRID_HEIGHT; y++) {
                if (grid[x][y] >= 4) {
                    topple(x, y);
                }
            }
        }
    }

    bool isStable() {
        for (int x = 0; x < Constants::GRID_WIDTH; x++) {
            for (int y = 0; y < Constants::GRID_HEIGHT; y++) {
                if (grid[x][y] >= 4) {
                    return false;
                }
            }
        }
        return true;
    }

    void updateGrid() {
        while (!isStable()) {
            toppleAll();
        }
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

    void clearGrid() {
        for (int x = 0; x < Constants::GRID_WIDTH; x++) {
            for (int y = 0; y < Constants::GRID_HEIGHT; y++) {
                grid[x][y] = 0;
            }
        }
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

const sf::Color SandPile::background = sf::Color(255, 255, 255);
const sf::Color SandPile::outline = sf::Color(0, 0, 0);
const sf::Color SandPile::green = sf::Color(0, 255, 0);
const sf::Color SandPile::purple = sf::Color(255, 0, 255);
const sf::Color SandPile::gold = sf::Color(255, 215, 0);
