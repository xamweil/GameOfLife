#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {
public:
    Grid(int n, int m);
    Grid(const Grid& other);
    Grid& operator=(const Grid& other);
    ~Grid();
    
    void setCell(int x, int y, int value);
    int getCell(int x, int y) const;
    std::vector<std::vector<int>> getGridData();
    std::vector<std::vector<int>> getInitialGridData();

    void toggleCell(int x, int y);

    int getN() const { return _n; }
    int getM() const { return _m; }
    void resize(int n, int m);
    void evolveGrid();
    int getNeighbors(int x, int y)const;
    void reset();
    void setInitialGrid(std::vector<std::vector<int>> initGrid);
private:
    int _n, _m;
    std::vector<std::vector<int>> gridData;  // 2D vector to store grid state
    std::vector<std::vector<int>> initialGridData; 
};

#endif // GRID_H

