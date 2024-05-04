#include "Grid.h"
#include <vector>

Grid::Grid(int n, int m) : _n(n), _m(m), gridData(n, std::vector<int>(m, 0)) {}

Grid::Grid(const Grid& other) : _n(other._n), _m(other._m), gridData(other.gridData){}

Grid::~Grid() {}

Grid& Grid::operator=(const Grid& other) {
    if (this != &other) {  // Protect against self-assignment
        _n = other._n;
        _m = other._m;
        gridData = other.gridData;  // vector's assignment operator does deep copy
        initialGridData = other.initialGridData;
    }
    return *this;
}

void Grid::setCell(int x, int y, int value) {
    gridData[x][y] = value;
}

int Grid::getCell(int x, int y) const {
    return gridData[x][y];
}

std::vector<std::vector<int>> Grid::getGridData(){
    return gridData;
}

std::vector<std::vector<int>> Grid::getInitialGridData(){
    return initialGridData;
}

void Grid::toggleCell(int x, int y) { //changed value of cell 
    gridData[x][y] = (gridData[x][y] == 0 ? 1 : 0);
}
//get number of neighbors
int Grid::getNeighbors(int x, int y) const{
    int count=0;
    for(int dx =-1; dx<=1; ++dx){
        for(int dy =-1; dy<=1; ++dy){
            if(dx==0 && dy==0) continue;
            int nx = x+dx;
            int ny = y+dy;
            if(nx >=0 && nx<_n && ny>=0 && ny<_m){
                count+=gridData[nx][ny];
            }
        }
    }
    return count;
}
void Grid::evolveGrid() {
    std::vector<std::vector<int>> buffer(_n, std::vector<int>(_m, 0));

    // Compute next state
    for (int i = 0; i < _n; i++) {
        for (int j = 0; j < _m; j++) {
            int numNeighbors = getNeighbors(i, j);
            if (gridData[i][j] == 1) {
                if (numNeighbors < 2 || numNeighbors > 3) {
                    buffer[i][j] = 0; // Die 
                } else {
                    buffer[i][j] = 1; // Live on 
                }
            } else {
                if (numNeighbors == 3) {
                    buffer[i][j] = 1; // Become alive
                }
            }
        }
    }

    gridData = buffer;
}

void Grid::reset(){
    gridData = initialGridData;
}

void Grid::setInitialGrid(std::vector<std::vector<int>> initGrid){
    initialGridData=initGrid;
}

void Grid::resize(int n, int m){
    _n = n;
    _m = m;
    gridData = std::vector<std::vector<int>> (n, std::vector<int>(m, 0));
    initialGridData = std::vector<std::vector<int>> (n, std::vector<int>(m, 0));
}