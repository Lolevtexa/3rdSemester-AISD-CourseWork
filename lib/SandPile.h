#pragma once
#include "Libraries.h"
#include "Constants.h"

class SandPile{
private:
    std::map<std::pair<int, int>, int> grid;

public:
    void addSand(int x, int y, int sandNumber) {
        grid[std::make_pair(x, y)] += sandNumber;
    }

    void updateGrid() {
        while (!isStable()) {
            toppleAll();
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
        grid[std::make_pair(x, y)] -= 4;
        grid[std::make_pair(x, y + 1)]++;
        grid[std::make_pair(x + 1, y)]++;
        grid[std::make_pair(x, y - 1)]++;
        grid[std::make_pair(x - 1, y)]++;
    }

    void toppleAll() {
        for (auto& [position, value] : grid) {
            if (value >= 4) {
                topple(position.first, position.second);
            }
        }
    }

    bool isStable() {
        for (const auto& [position, value] : grid) {
            if (value >= 4) {
                return false;
            }
        }
        return true;
    }
};
