#pragma once
#include "Libraries.h"

class Constants {
public:
    static const int WIDTH = 650;
    static const int HEIGHT = 600;

    static const int TOOL_BAR_WIDTH = 50;
    static const int TOOL_BAR_ICON_SIZE = 40;
    static const int TOOL_BAR_ICON_OUTLINE = (TOOL_BAR_WIDTH - TOOL_BAR_ICON_SIZE) / 2;

    static const int CELL_SIZE = 100;

    static const int GRID_WIDTH = (WIDTH - TOOL_BAR_WIDTH) / CELL_SIZE;
    static const int GRID_HEIGHT = HEIGHT / CELL_SIZE;

    static const int NUMBER_SELECTION_WIDTH = TOOL_BAR_WIDTH - 2 * TOOL_BAR_ICON_OUTLINE;
    static const int NUMBER_SELECTION_HEIGHT = 30;
    static const int NUMBER_SELECTION_TEXT_SIZE = 15;
    static const sf::Font NUMBER_SELECTION_FONT;
    static const sf::Color NUMBER_SELECTION_TEXT_COLOR;
};

const sf::Font Constants::NUMBER_SELECTION_FONT = []() {
    sf::Font font;
    if (font.loadFromFile("../assets/fonts/Arial.ttf")) {
        std::cout << "Font loaded successfully" << std::endl;
    } else {
        std::cout << "Font failed to load" << std::endl;
    }
    return font;
}();

const sf::Color Constants::NUMBER_SELECTION_TEXT_COLOR = sf::Color(0, 0, 0);
