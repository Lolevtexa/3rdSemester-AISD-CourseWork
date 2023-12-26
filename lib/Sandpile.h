#pragma once
#include "Libraries.h"
#include "Constants.h"

class Sandpile {
private:
    std::map<std::pair<int, int>, int> grid;
    std::set<std::pair<int, int>> unstableCells;

    int neighborsCount;

    std::vector<std::pair<int, int>> neighbors;

    std::vector<std::pair<int, int>> topNeighbors;
    std::vector<std::pair<int, int>> bottomNeighbors;

    bool oriented;

public:
    Sandpile(std::vector<std::pair<int, int>> neighbors) :
        neighbors(neighbors) {
        neighborsCount = neighbors.size();
        oriented = false;
    }

    Sandpile(std::vector<std::pair<int, int>> topNeighbors, std::vector<std::pair<int, int>> bottomNeighbors) :
        topNeighbors(topNeighbors),
        bottomNeighbors(bottomNeighbors) {
        neighborsCount = topNeighbors.size();
        oriented = true;
    }

    void addSand(int x, int y, int sandNumber) {
        grid[{x, y}] += sandNumber;
        if (grid[{x, y}] >= neighborsCount) {
            unstableCells.insert({x, y});
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
        if (grid.find({x, y}) == grid.end()) {
            return 0;
        } 
        return grid.at({x, y});
    }

private:
    void toppleNotOriented(int x, int y) {
        for (std::pair<int, int> neighbor : neighbors) {
            addSand(x + neighbor.first, y + neighbor.second, grid[{x, y}] / neighborsCount);
        }

        grid[{x, y}] %= neighborsCount;
    }

    void toppleOriented(int x, int y) {
        if ((x + y) % 2 == 0) {
            for (std::pair<int, int> neighbor : topNeighbors) {
                addSand(x + neighbor.first, y + neighbor.second, grid[{x, y}] / topNeighbors.size());
            }
        } else {
            for (std::pair<int, int> neighbor : bottomNeighbors) {
                addSand(x + neighbor.first, y + neighbor.second, grid[{x, y}] / bottomNeighbors.size());
            }
        }

        grid[{x, y}] %= topNeighbors.size();
    }

    void topple(int x, int y) {
        if (oriented) {
            toppleOriented(x, y);
        } else {
            toppleNotOriented(x, y);
        }
    }
};
