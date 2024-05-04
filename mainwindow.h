#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QSlider>
#include "Grid.h"
#include "GridEditor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Grid &grid, QWidget *parent = nullptr);
    void updateGridUI();  // Updates the UI based on grid state
    void onTimerTick();
    Grid editedGrid;
public slots:
    void applyGrid(const Grid& grid);
private slots:
    void startSimulation();
    void pauseSimulation();
    void restartSimulation();
    void updateSimulationSpeed(int interval);
    void openGridEditor();
    void regenerateGrid();
    
private:
    Grid& _grid;
    
    QVector<QWidget*> cellWidgets;  // Stores pointers to cell widgets
    QGridLayout *gridLayout;
    GridEditor *editor;
    QPushButton *gridEditorButton;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *restartButton;
    QSlider *speedSlider;
    QTimer *simulationTimer;
};

#endif // MAINWINDOW_H


