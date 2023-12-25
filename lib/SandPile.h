#pragma once
#include "Libraries.h"
#include "Constants.h"

class SandPile{
private:
    std::vector<std::vector<int>> grid;

public:
    SandPile(int width, int height) {
        grid.resize(width, std::vector<int>(height, 0));
    }
    
    void addSand(int x, int y, int sandNumber) {
        grid[x][y] += sandNumber;
    }

    void updateGrid() {
        while (!isStable()) {
            toppleAll();
        }
    }

    void clearGrid() {
        for (int x = 0; x < grid.size(); x++) {
            for (int y = 0; y < grid[x].size(); y++) {
                grid[x][y] = 0;
            }
        }
    }

    int getSandNumber(int x, int y) const {
        return grid[x][y];
    }
private:
    void topple(int x, int y) {
        grid[x][y] -= 4;
        if (y - 1 >= 0) grid[x][y - 1]++;
        if (y + 1 < grid[x].size()) grid[x][y + 1]++;
        if (x - 1 >= 0) grid[x - 1][y]++;
        if (x + 1 < grid.size()) grid[x + 1][y]++;
    }

    void toppleAll() {
        for (int x = 0; x < grid.size(); x++) {
            for (int y = 0; y < grid[x].size(); y++) {
                if (grid[x][y] >= 4) {
                    topple(x, y);
                }
            }
        }
    }

    bool isStable() {
        for (int x = 0; x < grid.size(); x++) {
            for (int y = 0; y < grid[x].size(); y++) {
                if (grid[x][y] >= 4) {
                    return false;
                }
            }
        }
        return true;
    }
};
