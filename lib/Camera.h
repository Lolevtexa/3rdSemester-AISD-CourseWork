#pragma once
#include "Libraries.h"
#include "Sandpile.h"

enum CellShape {
    TRIANGLE,
    RECTANGLE
};

class Camera : 
    public sf::RectangleShape {
private:
    static const sf::Color background;
    static const sf::Color outline;
    static const sf::Color green;
    static const sf::Color purple;
    static const sf::Color gold;
    
    Sandpile triangleSandpile;
    Sandpile rectangleSandpile;
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
        rectangleSandpile({{1, 0}, {0, 1}, {-1, 0}, {0, -1}}) {
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
        }
    }

    void updateGrid() {
        if (cellShape == TRIANGLE) {
            triangleSandpile.updateGrid();
        } else if (cellShape == RECTANGLE) {
            rectangleSandpile.updateGrid();
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
                        dy = sandRadiusWithTriangleCell() - sandRadiusWithTriangleCell() / 2.f;
                    } else {
                        dy = sandRadiusWithTriangleCell() + sandRadiusWithTriangleCell() / 2.f;
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

        sf::Vector2i topLeft = pointToRectangleGrid(getPosition() - getRectangleCell() / 2.f);
        sf::Vector2i bottomRight = pointToRectangleGrid(getPosition() + getSize() + getRectangleCell() / 2.f);

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
};

const sf::Color Camera::background = sf::Color(255, 255, 255);
const sf::Color Camera::outline = sf::Color(0, 0, 0);
const sf::Color Camera::green = sf::Color(0, 255, 0);
const sf::Color Camera::purple = sf::Color(255, 0, 255);
const sf::Color Camera::gold = sf::Color(255, 215, 0);
