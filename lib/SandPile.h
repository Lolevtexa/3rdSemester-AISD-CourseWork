#pragma once
#include "Libraries.h"
#include "Constants.h"

class SandPile{
private:
    std::map<std::pair<int, int>, int> grid;
    std::set<std::pair<int, int>> unstableCells;

public:
    void addSand(int x, int y, int sandNumber) {
        grid[std::make_pair(x, y)] += sandNumber;
        if (grid[std::make_pair(x, y)] >= 4) {
            unstableCells.insert(std::make_pair(x, y));
        }
    }

    void updateGrid() {
        while (!unstableCells.empty()) {
            std::pair<int, int> cell = *unstableCells.begin();
            topple(cell.first, cell.second);
            unstableCells.erase(cell);
        }
    }

    void clearGrid() {
        grid.clear();
    }

    int getSandNumber(int x, int y) const {
        if (grid.find(std::make_pair(x, y)) == grid.end()) {
            return 0;
        } 
        return grid.at(std::make_pair(x, y));
    }
private:
    void topple(int x, int y) {
        addSand(x, y + 1, grid[std::make_pair(x, y)] / 4);
        addSand(x + 1, y, grid[std::make_pair(x, y)] / 4);
        addSand(x, y - 1, grid[std::make_pair(x, y)] / 4);
        addSand(x - 1, y, grid[std::make_pair(x, y)] / 4);
        grid[std::make_pair(x, y)] %= 4;
    }
};
