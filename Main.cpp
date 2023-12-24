#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 800;
const int CELL_SIZE = 10;

const int GRID_WIDTH = WIDTH / CELL_SIZE;
const int GRID_HEIGHT = HEIGHT / CELL_SIZE;

class SandPile {
private:
    std::vector<std::vector<int>> grid;

    static const sf::Color background;
    static const sf::Color outline;
    static const sf::Color green;
    static const sf::Color purple;
    static const sf::Color gold;

public:
    SandPile() {
        grid.resize(GRID_WIDTH, std::vector<int>(GRID_HEIGHT, 0));
    }

    void eventProcessing(sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                addSand(event.mouseButton.x / CELL_SIZE, event.mouseButton.y / CELL_SIZE);
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                updateGrid();
            }

            if (event.key.code == sf::Keyboard::S) {
                saveScreenshot("screenshot.png", window);
            }
        }
    }

    void drawGrid(sf::RenderWindow& window) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setFillColor(background);
                cell.setOutlineColor(outline);
                cell.setOutlineThickness(-0.5f);
                window.draw(cell);

                if (grid[x][y] > 0) {
                    sf::CircleShape sand(CELL_SIZE / 2);
                    sand.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    sand.setFillColor(grad(grid[x][y]));
                    window.draw(sand);
                }
            }
        }
    }

    void saveScreenshot(const std::string& filename, sf::RenderWindow& window) {
        sf::Texture texture;
        texture.create(window.getSize().x, window.getSize().y);
        texture.update(window);
        if (texture.copyToImage().saveToFile(filename)) {
            std::cout << "Screenshot saved to " << filename << std::endl;
        } else {
            std::cout << "Failed to save screenshot" << std::endl;
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

    void updateGrid() {
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                if (grid[x][y] >= 4) {
                    topple(x, y);
                }
            }
        }
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

    SandPile sandPile;

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
