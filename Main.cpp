#include <SFML/Graphics.hpp>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 800;
const int CELL_SIZE = 10;

const int GRID_WIDTH = WIDTH / CELL_SIZE;
const int GRID_HEIGHT = HEIGHT / CELL_SIZE;

class SandPile {
protected:
    std::vector<std::vector<int>> grid;

public:
    SandPile() {
        grid.resize(GRID_WIDTH, std::vector<int>(GRID_HEIGHT, 0));
    }

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

    void drawGrid(sf::RenderWindow& window) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setFillColor(sf::Color::White);
                cell.setOutlineColor(sf::Color::Black);
                cell.setOutlineThickness(1.0f);
                window.draw(cell);

                if (grid[x][y] > 0) {
                    sf::CircleShape sand(CELL_SIZE / 2);
                    sand.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    sand.setFillColor(sf::Color::Yellow);
                    window.draw(sand);
                }
            }
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Sandpile Model");
    window.setPosition(sf::Vector2i(0, 0));

    SandPile sandPile;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / CELL_SIZE;
                int y = event.mouseButton.y / CELL_SIZE;
                sandPile.addSand(x, y);
            }
        }

        sandPile.updateGrid();

        window.clear();
        sandPile.drawGrid(window);
        window.display();
    }

    return 0;
}
