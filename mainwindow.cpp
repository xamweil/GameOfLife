#include "MainWindow.h"
#include <QGridLayout>
#include <QWidget>
#include <QSlider>
#include "Grid.h"
#include <QSpacerItem>
#include "GridEditor.h"
#include <iostream>
#include <QApplication>

MainWindow::MainWindow(Grid &grid, QWidget *parent) : QMainWindow(parent), _grid(grid), editedGrid(grid){
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    editor = new GridEditor(editedGrid, this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    gridLayout = new QGridLayout();
    mainLayout->addLayout(gridLayout);

    cellWidgets.resize(_grid.getN() * _grid.getM());

    for (int i = 0; i < _grid.getN(); i++) {
        for (int j = 0; j < _grid.getM(); j++) {
            QWidget *cell = new QWidget();
            cell->setFixedSize(10, 10);  // Size adjustment for visibility
            cellWidgets[i * _grid.getM() + j] = cell;
            gridLayout->addWidget(cell, i, j);
        }
    }

    // Button setup
    QHBoxLayout *MenuLayout = new QHBoxLayout();
    QVBoxLayout *verticalButtonLayout = new QVBoxLayout();
    
    startButton = new QPushButton("Start");
    pauseButton = new QPushButton("Pause");
    restartButton = new QPushButton("Restart");
    gridEditorButton = new QPushButton("Grid Editor");

    //Button size
    startButton->setFixedSize(150, 80);
    pauseButton->setFixedSize(100, 40);
    restartButton->setFixedSize(100, 40);
    gridEditorButton->setFixedSize(150, 80);

    //slider
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(0,790);
    speedSlider->setValue(500);


    //layout of menu
    verticalButtonLayout->addWidget(pauseButton);
    verticalButtonLayout->addWidget(restartButton);
    MenuLayout->addWidget(gridEditorButton);
    MenuLayout->addWidget(speedSlider);
    MenuLayout->addLayout(verticalButtonLayout);
    //buttonsLayout->addStretch(1);
    MenuLayout->addWidget(startButton);
    

    startButton->setStyleSheet("QPushButton { background-color: green; border-radius: 20px; }");
    pauseButton->setStyleSheet("QPushButton {border-radius: 20px; }");
    restartButton->setStyleSheet("QPushButton {border-radius: 20px; }");

    mainLayout->addLayout(MenuLayout);

    // Connect buttons to slots
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseSimulation);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartSimulation);
    connect(gridEditorButton, &QPushButton::clicked, this, &MainWindow::openGridEditor);
    //connect slider
    connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::updateSimulationSpeed);
    //connect from editor;
    connect(editor, &GridEditor::applyGridRequest, this, &MainWindow::applyGrid);
    // Timer setup
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::onTimerTick);

    updateGridUI();
}

void MainWindow::updateGridUI() {
    for (int i = 0; i < _grid.getN(); i++) {
        for (int j = 0; j < _grid.getM(); j++) {
            int index = i * _grid.getM() + j;
            if (_grid.getCell(i, j) == 1) {
                cellWidgets[index]->setStyleSheet("background-color: black;");
            } else {
                cellWidgets[index]->setStyleSheet("background-color: white;");
            }
        }
    }
}
//evolution of grid
void MainWindow::onTimerTick(){
    _grid.evolveGrid();
    updateGridUI();
}

void MainWindow::startSimulation() {
    if (!simulationTimer->isActive()) {
        simulationTimer->start(500); 
    }
}


void MainWindow::pauseSimulation() {
    if (simulationTimer->isActive()) {
        simulationTimer->stop();
    }
}
//loads initial grid
void MainWindow::restartSimulation() {
    _grid.reset();  
    updateGridUI();
}

void MainWindow::updateSimulationSpeed(int interval){
    simulationTimer->setInterval(800-interval);
}

void MainWindow::openGridEditor() {
    
    editor->exec();  // This makes the dialog modal
}

void MainWindow::applyGrid(const Grid& grid){
    _grid = grid;
    std::cout<<_grid.getM()<<" "<< _grid.getN()<< " "<<_grid.getGridData().size()<< " "<<_grid.getInitialGridData().size()<<std::endl;
    editor->close();
    regenerateGrid();
    
}

void MainWindow::regenerateGrid() {

    int currentCount = cellWidgets.size();
    int newCount = _grid.getN() * _grid.getM();

    if (currentCount != newCount) {
        // If the grid size has changed, we need to adjust the widgets.

        // Clear existing widgets
        std::cout<<"Delete widgets"<<std::endl;
        QLayoutItem* item;
        while ((item = gridLayout->takeAt(0)) != nullptr) {
            QWidget* widget = item->widget();
            if (widget) {
                widget->deleteLater();
            }
            delete item;
        }
        std::cout<<"clear widgets"<<std::endl;
        cellWidgets.clear();

        // Resize the vector to accommodate new widgets
        cellWidgets.resize(newCount);
        std::cout<<"make new widgets"<<std::endl;

        // Create new widgets as per new grid dimensions
        for (int i = 0; i < _grid.getN(); i++) {
            for (int j = 0; j < _grid.getM(); j++) {
                QWidget *cell = new QWidget();
                cell->setFixedSize(10, 10);  // Set fixed size for visibility
                gridLayout->addWidget(cell, i, j);
                int index = i * _grid.getM() + j;
                cellWidgets[index] = cell;
            }
        }

    }

    // update UI.
    updateGridUI();
    this->update();
    QApplication::processEvents();
    this->resize(minimumSize());
 
}

