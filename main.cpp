#include <QApplication>
#include "mainwindow.h"
#include "Grid.h"
#include "GridEditor.h"
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Grid grid(40, 60);
    grid.setCell(19,30,1);
    grid.setCell(20,30,1);
    grid.setCell(21,30,1); 

    grid.setCell(1,2,1);
    grid.setCell(2,3,1);
    grid.setCell(3,3,1);
    grid.setCell(3,2,1);
    grid.setCell(3,1,1);
    grid.setInitialGrid(grid.getGridData());
    MainWindow mainWindow(grid);
    mainWindow.show();
    
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        grid.evolveGrid();
        mainWindow.updateGridUI();
    });

    
    return app.exec();
    

}



