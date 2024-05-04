#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H


#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include "Grid.h"
#include <QSignalMapper>

class GridEditor : public QDialog {
    Q_OBJECT

    public:
        explicit GridEditor(Grid& grid, QWidget *parent = nullptr);
        Grid& getGrid();
        

    signals:
        void applyGridRequest(const Grid& grid);
    
    public slots:
        void applyButtonClick();

    private slots:
        void saveGrid();
        void loadGrid();
        void updateGridSize();
        void cellClicked(int index);
        void updateGridUI();
        void resizeGrid();
        void generateGrid();
        QString getStyleSheet(int i, int j);
    private:
        QVector<QPushButton*> cellButtons;
        Grid& _grid;
        QHBoxLayout* _menuLayout;
        QVBoxLayout* _mainLayout;
        QVBoxLayout* _resizeLayout1;
        QHBoxLayout* _resizeLayout2;
        QGridLayout* _gridLayout;
        QPushButton* _saveButton;
        QPushButton* _loadButton;
        QPushButton* _applyButton;
        QPushButton* _resizeButton;
        QSpinBox* _gridWidthBox;
        QSpinBox* _gridHeightBox;

        QSignalMapper* mapper;
        
        
};

#endif