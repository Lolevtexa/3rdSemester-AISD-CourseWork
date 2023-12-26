#pragma once
#include "Libraries.h"
#include "Constants.h"

class Sandpile {
private:
    std::map<std::pair<int, int>, int> grid;
    std::set<std::pair<int, int>> unstableCells;

    std::vector<std::pair<int, int>> neighbors;

public:
    Sandpile(std::vector<std::pair<int, int>> neighbors) :
        neighbors(neighbors) {
    }

    void addSand(int x, int y, int sandNumber) {
        grid[std::make_pair(x, y)] += sandNumber;
        if (grid[std::make_pair(x, y)] >= neighbors.size()) {
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
        for (std::pair<int, int> neighbor : neighbors) {
            addSand(x + neighbor.first, y + neighbor.second, grid[std::make_pair(x, y)] / neighbors.size());
        }

        grid[std::make_pair(x, y)] %= neighbors.size();
    }
};
