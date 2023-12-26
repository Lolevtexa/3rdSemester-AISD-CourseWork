#pragma once
#include "Libraries.h"
#include "Sandpile.h"

enum CellShape {
    TRIANGLE,
    RECTANGLE,
    HEXAGON
};

class Camera : 
    public sf::RectangleShape {
private:
    static const sf::Color background;
    static const sf::Color outline;
    static const sf::Color green;
    static const sf::Color purple;
    static const sf::Color gold;
    static const sf::Color red;
    static const sf::Color blue;
    static const sf::Color black;
    
    Sandpile triangleSandpile;
    Sandpile rectangleSandpile;
    Sandpile hexagonSandpile;

    CellShape cellShape;

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
    Camera(sf::Vector2f position, sf::Vector2f size, float cellSize) : 
        cellSize(cellSize),
        triangleSandpile({{1, 0}, {-1, 0}, {0, 1}}, {{1, 0}, {-1, 0}, {0, -1}}),
        rectangleSandpile({{1, 0}, {0, 1}, {-1, 0}, {0, -1}}), 
        hexagonSandpile({{0, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}}) {
        setPosition(position);
        setSize(size);
        setFillColor(background);
    }

    void setShape(CellShape cellShape) {
        this->cellShape = cellShape;
    }

    void clearGrid() {
        if (cellShape == TRIANGLE) {
            triangleSandpile.clearGrid();
        } else if (cellShape == RECTANGLE) {
            rectangleSandpile.clearGrid();
        } else if (cellShape == HEXAGON) {
            hexagonSandpile.clearGrid();
        }
    }

    void updateGrid() {
        if (cellShape == TRIANGLE) {
            triangleSandpile.updateGrid();
        } else if (cellShape == RECTANGLE) {
            rectangleSandpile.updateGrid();
        } else if (cellShape == HEXAGON) {
            hexagonSandpile.updateGrid();
        }
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
                addSand(globalMousePosition, func());
            }
            needAddSand = false;
        }

        if (needClearGrid) {
            rectangleSandpile.clearGrid();
            needClearGrid = false;
        }

        if (updateSandpile) {
            rectangleSandpile.updateGrid();
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

        if (cellShape == TRIANGLE) {
            drawTriangleSandpile(target, states);
        } else if (cellShape == RECTANGLE) {
            drawRectangleSandpile(target, states);
        } else if (cellShape == HEXAGON) {
            drawHexagonSandpile(target, states);
        }
    }

private:
    bool containsTriangle(sf::ConvexShape triangle, sf::Vector2f point) const {
        sf::Vector2f A = triangle.getPoint(0) + triangle.getPosition();
        sf::Vector2f B = triangle.getPoint(1) + triangle.getPosition();
        sf::Vector2f C = triangle.getPoint(2) + triangle.getPosition();
        sf::Vector2f P = point;

        float AB = (A.x - P.x) * (B.y - P.y) - (B.x - P.x) * (A.y - P.y);
        float BC = (B.x - P.x) * (C.y - P.y) - (C.x - P.x) * (B.y - P.y);
        float CA = (C.x - P.x) * (A.y - P.y) - (A.x - P.x) * (C.y - P.y);

        return (AB >= 0 && BC >= 0 && CA >= 0) || (AB <= 0 && BC <= 0 && CA <= 0);
    }

    bool containsHexagon(sf::ConvexShape hexagon, sf::Vector2f point) const {
        sf::Vector2f A = hexagon.getPoint(0) + hexagon.getPosition();
        sf::Vector2f B = hexagon.getPoint(1) + hexagon.getPosition();
        sf::Vector2f C = hexagon.getPoint(2) + hexagon.getPosition();
        sf::Vector2f D = hexagon.getPoint(3) + hexagon.getPosition();
        sf::Vector2f E = hexagon.getPoint(4) + hexagon.getPosition();
        sf::Vector2f F = hexagon.getPoint(5) + hexagon.getPosition();
        sf::Vector2f P = point;

        float AB = (A.x - P.x) * (B.y - P.y) - (B.x - P.x) * (A.y - P.y);
        float BC = (B.x - P.x) * (C.y - P.y) - (C.x - P.x) * (B.y - P.y);
        float CD = (C.x - P.x) * (D.y - P.y) - (D.x - P.x) * (C.y - P.y);
        float DE = (D.x - P.x) * (E.y - P.y) - (E.x - P.x) * (D.y - P.y);
        float EF = (E.x - P.x) * (F.y - P.y) - (F.x - P.x) * (E.y - P.y);
        float FA = (F.x - P.x) * (A.y - P.y) - (A.x - P.x) * (F.y - P.y);

        return (AB >= 0 && BC >= 0 && CD >= 0 && DE >= 0 && EF >= 0 && FA >= 0) || (AB <= 0 && BC <= 0 && CD <= 0 && DE <= 0 && EF <= 0 && FA <= 0);
    }

    void addSand(sf::Vector2f globalMousePosition, int sandNumber) {
        if (cellShape == TRIANGLE) {
            sf::ConvexShape topTriangle = setTopTriangle();
            sf::ConvexShape bottomTriangle = setBottomTriangle();

            int x = pointToTriangleGrid(globalMousePosition).x;
            int y = pointToTriangleGrid(globalMousePosition).y;
            if ((x + y) % 2 == 0) {
                topTriangle.setPosition(triangleGridToPoint(x, y));
                if (!containsTriangle(topTriangle, globalMousePosition)) {
                    bottomTriangle.setPosition(triangleGridToPoint(x + 1, y));
                    if (containsTriangle(bottomTriangle, globalMousePosition)) {
                        x++;
                    } else {
                        x--;
                    }
                }
            } else {
                bottomTriangle.setPosition(triangleGridToPoint(x, y));
                if (!containsTriangle(bottomTriangle, globalMousePosition)) {
                    topTriangle.setPosition(triangleGridToPoint(x + 1, y));
                    if (containsTriangle(topTriangle, globalMousePosition)) {
                        x++;
                    } else {
                        x--;
                    }
                }
            }

            triangleSandpile.addSand(x, y, sandNumber);
        } else if (cellShape == RECTANGLE) {
            int x = pointToRectangleGrid(globalMousePosition).x;
            int y = pointToRectangleGrid(globalMousePosition).y;
            
            rectangleSandpile.addSand(x, y, sandNumber);
        } else if (cellShape == HEXAGON) {
            sf::ConvexShape hexagon = setHexagon();

            int x = pointToHexagonGrid(globalMousePosition).x;
            int y = pointToHexagonGrid(globalMousePosition).y;
            if (x % 2 == 0) {
                hexagon.setPosition(hexagonGridToPoint(x, y));
                if (!containsHexagon(hexagon, globalMousePosition)) {
                    hexagon.setPosition(hexagonGridToPoint(x + 1, y) - sf::Vector2f(0, getHexagonCell().y / 2.f));
                    if (containsHexagon(hexagon, globalMousePosition)) {
                        x++;
                    } else {
                        hexagon.setPosition(hexagonGridToPoint(x - 1, y) - sf::Vector2f(0, getHexagonCell().y / 2.f));
                        if (containsHexagon(hexagon, globalMousePosition)) {
                            x--;
                        } else {
                            hexagon.setPosition(hexagonGridToPoint(x + 1, y + 1) - sf::Vector2f(0, getHexagonCell().y / 2.f));
                            if (containsHexagon(hexagon, globalMousePosition)) {
                                x++;
                                y++;
                            } else {
                                x--;
                                y++;
                            }
                        }
                    }
                }
            } else {
                hexagon.setPosition(hexagonGridToPoint(x, y));
                if (!containsHexagon(hexagon, globalMousePosition - sf::Vector2f(0, getHexagonCell().y / 2.f))) {
                    hexagon.setPosition(hexagonGridToPoint(x + 1, y));
                    if (containsHexagon(hexagon, globalMousePosition)) {
                        x++;
                    } else {
                        hexagon.setPosition(hexagonGridToPoint(x - 1, y));
                        if (containsHexagon(hexagon, globalMousePosition)) {
                            x--;
                        } else {
                            hexagon.setPosition(hexagonGridToPoint(x + 1, y - 1));
                            if (containsHexagon(hexagon, globalMousePosition)) {
                                x++;
                                y--;
                            } else {
                                x--;
                                y--;
                            }
                        }
                    }
                }
            }

            hexagonSandpile.addSand(x, y, sandNumber);
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

    sf::Color gradTriangle(int value) const {
        if (value == 0) {
            return background;
        } else if (value == 1) {
            return purple;
        } else if (value == 2) {
            return gold;
        }
        return black;
    }

    sf::Color gradRectangle(int value) const {
        if (value == 0) {
            return background;
        } else if (value == 1) {
            return green;
        } else if (value == 2) {
            return purple;
        } else if (value == 3) {
            return gold;
        }
        return black;
    }

    sf::Color gradHexagon(int value) const {
        if (value == 0) {
            return background;
        } else if (value == 1) {
            return green;
        } else if (value == 2) {
            return purple;
        } else if (value == 3) {
            return gold;
        } else if (value == 4) {
            return red;
        } else if (value == 5) {
            return blue;
        }
        return black;
    }

    sf::Color grad(int value) const {
        if (cellShape == TRIANGLE) {
            return gradTriangle(value);
        } else if (cellShape == RECTANGLE) {
            return gradRectangle(value);
        } else if (cellShape == HEXAGON) {
            return gradHexagon(value);
        }
        return black;
    }
    
    sf::Vector2f delta() const {
        return getPosition() + getSize() / 2.f - position;
    }

    sf::ConvexShape setTopTriangle() const {
        sf::ConvexShape topTriangle(3);
        topTriangle.setPoint(0, sf::Vector2f(0, -getTriangleCell().y / 2));
        topTriangle.setPoint(1, sf::Vector2f(getTriangleCell().x, getTriangleCell().y / 2));
        topTriangle.setPoint(2, sf::Vector2f(-getTriangleCell().x, getTriangleCell().y / 2));
        topTriangle.setFillColor(background);
        topTriangle.setOutlineColor(outline);
        topTriangle.setOutlineThickness(-1.f);
        return topTriangle;
    }

    sf::ConvexShape setBottomTriangle() const {
        sf::ConvexShape bottomTriangle(3);
        bottomTriangle.setPoint(0, sf::Vector2f(0, getTriangleCell().y / 2));
        bottomTriangle.setPoint(1, sf::Vector2f(getTriangleCell().x, -getTriangleCell().y / 2));
        bottomTriangle.setPoint(2, sf::Vector2f(-getTriangleCell().x, -getTriangleCell().y / 2));
        bottomTriangle.setFillColor(background);
        bottomTriangle.setOutlineColor(outline);
        bottomTriangle.setOutlineThickness(-1.f);
        return bottomTriangle;
    }

    sf::Vector2f getTriangleCell() const {
        return sf::Vector2f(cellSize * scale, cellSize * scale * sqrt(3)) / 2.f;
    }

    sf::Vector2f getTriangleDelta() const {
        return getPosition() + getSize() / 2.f - position;
    }

    sf::Vector2i pointToTriangleGrid(sf::Vector2f _mousePosition) const {
        int x = std::round((_mousePosition - getTriangleDelta()).x / getTriangleCell().x);
        int y = std::round((_mousePosition - getTriangleDelta()).y / getTriangleCell().y);
        return sf::Vector2i(x, y);
    }

    sf::Vector2f triangleGridToPoint(int _x, int _y) const {
        float x = (float)_x * getTriangleCell().x + getTriangleDelta().x;
        float y = (float)_y * getTriangleCell().y + getTriangleDelta().y;
        return sf::Vector2f(x, y);
    }

    float sandRadiusWithTriangleCell() const {
        return cellSize * scale * sqrt(3) / 2.f / 3.f;
    }

    void drawTriangleSandpile(sf::RenderTarget& target, sf::RenderStates states) const {
        sf::ConvexShape topTriangle = setTopTriangle();
        sf::ConvexShape bottomTriangle = setBottomTriangle();

        sf::Vector2i topLeft = pointToTriangleGrid(getPosition() - getTriangleCell());
        sf::Vector2i bottomRight = pointToTriangleGrid(getPosition() + getSize() + getTriangleCell());

        for (int x = topLeft.x; x < bottomRight.x; x++) {
            for (int y = topLeft.y; y < bottomRight.y; y++) {
                float _x = triangleGridToPoint(x, y).x;
                float _y = triangleGridToPoint(x, y).y;
                if ((x + y) % 2 == 0) {
                    topTriangle.setPosition(_x, _y);
                    target.draw(topTriangle, states);
                } else {
                    bottomTriangle.setPosition(_x, _y);
                    target.draw(bottomTriangle, states);
                }
            }
        }

        sf::CircleShape sand(sandRadiusWithTriangleCell());
        float dx = sandRadiusWithTriangleCell();
        float dy = sandRadiusWithTriangleCell();
        for (int x = topLeft.x; x < bottomRight.x; x++) {
            for (int y = topLeft.y; y < bottomRight.y; y++) {
                if (triangleSandpile.getSandNumber(x, y) > 0) {
                    if ((x + y) % 2 == 0) {
                        dy = sandRadiusWithTriangleCell() / 2.f;
                    } else {
                        dy = sandRadiusWithTriangleCell() * 3.f / 2.f;
                    }
                    sand.setPosition(triangleGridToPoint(x, y) - sf::Vector2f(dx, dy));
                    sand.setFillColor(grad(triangleSandpile.getSandNumber(x, y)));
                    target.draw(sand, states);
                }
            }
        }
    }

    sf::ConvexShape setRectangle() const {
        sf::ConvexShape rectangle(4);
        rectangle.setPoint(0, sf::Vector2f(-getRectangleCell().x / 2.f, -getRectangleCell().y / 2.f));
        rectangle.setPoint(1, sf::Vector2f(getRectangleCell().x / 2.f, -getRectangleCell().y / 2.f));
        rectangle.setPoint(2, sf::Vector2f(getRectangleCell().x / 2.f, getRectangleCell().y / 2.f));
        rectangle.setPoint(3, sf::Vector2f(-getRectangleCell().x / 2.f, getRectangleCell().y / 2.f));
        rectangle.setFillColor(background);
        rectangle.setOutlineColor(outline);
        rectangle.setOutlineThickness(-1.f);
        return rectangle;
    }

    sf::Vector2f getRectangleCell() const {
        return sf::Vector2f(cellSize * scale, cellSize * scale);
    }
    
    sf::Vector2i pointToRectangleGrid(sf::Vector2f point) const {
        int x = std::round((point - delta()).x / getRectangleCell().x);
        int y = std::round((point - delta()).y / getRectangleCell().y);
        return sf::Vector2i(x, y);
    }

    sf::Vector2f rectangleGridToPoint(int _x, int _y) const {
        float x = (float)_x * getRectangleCell().x + delta().x;
        float y = (float)_y * getRectangleCell().y + delta().y;
        return sf::Vector2f(x, y);
    }

    float sandRadiusWithRectangleCell() const {
        return cellSize * scale / 2.f;
    }

    void drawRectangleSandpile(sf::RenderTarget& target, sf::RenderStates states) const {
        sf::ConvexShape rectangle = setRectangle();

        sf::Vector2i topLeft = pointToRectangleGrid(getPosition() - getRectangleCell());
        sf::Vector2i bottomRight = pointToRectangleGrid(getPosition() + getSize() + getRectangleCell());

        for (int x = topLeft.x; x < bottomRight.x; x++) {
            for (int y = topLeft.y; y < bottomRight.y; y++) {
                rectangle.setPosition(rectangleGridToPoint(x, y));
                target.draw(rectangle, states);
            }
        }

        sf::CircleShape sand(sandRadiusWithRectangleCell());
        float dx = sandRadiusWithRectangleCell();
        float dy = sandRadiusWithRectangleCell();
        for (int x = topLeft.x; x < bottomRight.x; x++) {
            for (int y = topLeft.y; y < bottomRight.y; y++) {
                if (rectangleSandpile.getSandNumber(x, y) > 0) {
                    sand.setPosition(rectangleGridToPoint(x, y) - sf::Vector2f(dx, dy));
                    sand.setFillColor(grad(rectangleSandpile.getSandNumber(x, y)));
                    target.draw(sand, states);
                }
            }
        }
    }

    sf::ConvexShape setHexagon() const {
        sf::ConvexShape hexagon(6);
        hexagon.setPoint(0, sf::Vector2f(-getHexagonCell().x / 3.f, -getHexagonCell().y / 2.f));
        hexagon.setPoint(1, sf::Vector2f(getHexagonCell().x / 3.f, -getHexagonCell().y / 2.f));
        hexagon.setPoint(2, sf::Vector2f(getHexagonCell().x * 2.f / 3.f, 0));
        hexagon.setPoint(3, sf::Vector2f(getHexagonCell().x  / 3.f, getHexagonCell().y / 2.f));
        hexagon.setPoint(4, sf::Vector2f(-getHexagonCell().x / 3.f, getHexagonCell().y / 2.f));
        hexagon.setPoint(5, sf::Vector2f(-getHexagonCell().x * 2.f / 3.f, 0));
        hexagon.setFillColor(background);
        hexagon.setOutlineColor(outline);
        hexagon.setOutlineThickness(-1.f);
        return hexagon;
    }

    sf::Vector2f getHexagonCell() const {
        return sf::Vector2f(cellSize * scale * 3.f / 2.f, cellSize * scale * sqrt(3));
    }

    sf::Vector2i pointToHexagonGrid(sf::Vector2f point) const {
        int x = std::round((point - delta()).x / getHexagonCell().x);
        int y = std::round((point - delta()).y / getHexagonCell().y);
        return sf::Vector2i(x, y);
    }

    sf::Vector2f hexagonGridToPoint(int _x, int _y) const {
        float x = (float)_x * getHexagonCell().x + delta().x;
        float y = (float)_y * getHexagonCell().y + delta().y;
        return sf::Vector2f(x, y);
    }

    float sandRadiusWithHexagonCell() const {
        return cellSize * scale * sqrt(3) / 2.f;
    }

    void drawHexagonSandpile(sf::RenderTarget& target, sf::RenderStates states) const {
        sf::ConvexShape hexagon = setHexagon();

        sf::Vector2i topLeft = pointToHexagonGrid(getPosition() - getHexagonCell());
        sf::Vector2i bottomRight = pointToHexagonGrid(getPosition() + getSize() + getHexagonCell());

        for (int x = topLeft.x; x < bottomRight.x; x++) {
            for (int y = topLeft.y; y < bottomRight.y; y++) {
                if (x % 2 == 0) {
                    hexagon.setPosition(hexagonGridToPoint(x, y));
                    target.draw(hexagon, states);
                } else {
                    hexagon.setPosition(hexagonGridToPoint(x, y) - sf::Vector2f(0, getHexagonCell().y / 2.f));
                    target.draw(hexagon, states);
                }
            }
        }

        sf::CircleShape sand(sandRadiusWithHexagonCell());
        float dx = sandRadiusWithHexagonCell();
        float dy = sandRadiusWithHexagonCell();
        for (int x = topLeft.x; x < bottomRight.x; x++) {
            for (int y = topLeft.y; y < bottomRight.y; y++) {
                if (hexagonSandpile.getSandNumber(x, y) > 0) {
                    if (x % 2 == 0) {
                        dy = sandRadiusWithHexagonCell();
                    } else {
                        dy = sandRadiusWithHexagonCell() * 2.f;
                    }
                    sand.setPosition(hexagonGridToPoint(x, y) - sf::Vector2f(dx, dy));
                    sand.setFillColor(grad(hexagonSandpile.getSandNumber(x, y)));
                    target.draw(sand, states);
                }
            }
        }
    }
};

const sf::Color Camera::background = sf::Color(255, 255, 255);
const sf::Color Camera::outline = sf::Color(0, 0, 0);
const sf::Color Camera::green = sf::Color(0, 255, 0);
const sf::Color Camera::purple = sf::Color(255, 0, 255);
const sf::Color Camera::gold = sf::Color(255, 215, 0);
const sf::Color Camera::red = sf::Color(255, 0, 0);
const sf::Color Camera::blue = sf::Color(0, 0, 255);
const sf::Color Camera::black = sf::Color(0, 0, 0);
