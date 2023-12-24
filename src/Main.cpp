#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>

const int WIDTH = 650;
const int HEIGHT = 600;
const int TOOL_BAR_WIDTH = 50;
const int CELL_SIZE = 10;

const int GRID_WIDTH = (WIDTH - TOOL_BAR_WIDTH) / CELL_SIZE;
const int GRID_HEIGHT = HEIGHT / CELL_SIZE;

class Button {
private:
    sf::RectangleShape button;
    sf::Image icon;
    sf::Texture texture;

    std::function<void()> eventButtonPressed;
public:
    Button(const std::string& filename, int x, int y, int width = 40, int height = 40) {
        icon.loadFromFile(filename);
        texture.loadFromImage(icon);

        button.setSize(sf::Vector2f(width, height));
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

class ToolBar {
private:
    static const sf::Color background;
    static const sf::Color outline;

    std::vector<Button> buttons;

public:
    template<typename Func>
    void addButton(const std::string& filename, Func func) {
        int y = 5 + buttons.size() * 50;
        Button button(filename, 5, y);
        button.setEventButtonPressed(func);
        buttons.push_back(button);
    }

    void eventProcessing(sf::Event event) {
        for (Button& button : buttons) {
            button.eventProcessing(event);
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::RectangleShape toolBar(sf::Vector2f(TOOL_BAR_WIDTH, HEIGHT));
        toolBar.setPosition(0, 0);
        toolBar.setFillColor(background);
        toolBar.setOutlineColor(outline);
        toolBar.setOutlineThickness(-0.5f);
        window.draw(toolBar);

        for (Button& button : buttons) {
            button.draw(window);
        }
    }
};

const sf::Color ToolBar::background = sf::Color(200, 200, 200);
const sf::Color ToolBar::outline = sf::Color(0, 0, 0);

class SandPile {
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

        grid.resize(GRID_WIDTH, std::vector<int>(GRID_HEIGHT, 0));
    }

    void eventProcessing(sf::Event& event, sf::RenderWindow& window) {
        toolBar.eventProcessing(event);

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (event.mouseButton.x >= TOOL_BAR_WIDTH) {
                    addSand((event.mouseButton.x - TOOL_BAR_WIDTH) / CELL_SIZE, event.mouseButton.y / CELL_SIZE);
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

    void drawGrid(sf::RenderWindow& window) {
        toolBar.draw(window);

        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(x * CELL_SIZE + TOOL_BAR_WIDTH, y * CELL_SIZE);
                cell.setFillColor(background);
                cell.setOutlineColor(outline);
                cell.setOutlineThickness(-0.5f);
                window.draw(cell);

                if (grid[x][y] > 0) {
                    sf::CircleShape sand(CELL_SIZE / 2);
                    sand.setPosition(x * CELL_SIZE + TOOL_BAR_WIDTH, y * CELL_SIZE);
                    sand.setFillColor(grad(grid[x][y]));
                    window.draw(sand);
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
        if (y + 1 < GRID_HEIGHT) grid[x][y + 1]++;
        if (x - 1 >= 0) grid[x - 1][y]++;
        if (x + 1 < GRID_WIDTH) grid[x + 1][y]++;
    }

    void toppleAll() {
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                if (grid[x][y] >= 4) {
                    topple(x, y);
                }
            }
        }
    }

    bool isStable() {
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
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

    sf::Color grad(int value) {
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

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Sandpile Model");
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
        sandPile.drawGrid(window);
        window.display();
    }

    return 0;
}
