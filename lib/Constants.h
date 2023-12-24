#pragma once
#include "Libraries.h"

class Constants {
public:
    static const int WIDTH = 650;
    static const int HEIGHT = 600;

    static const int TOOL_BAR_WIDTH = 50;
    static const int TOOL_BAR_ICON_SIZE = 40;
    static const int TOOL_BAR_ICON_OUTLINE = (TOOL_BAR_WIDTH - TOOL_BAR_ICON_SIZE) / 2;

    static const int CELL_SIZE = 10;

    static const int GRID_WIDTH = (WIDTH - TOOL_BAR_WIDTH) / CELL_SIZE;
    static const int GRID_HEIGHT = HEIGHT / CELL_SIZE;
};
