#include "GridEditor.h"
#include "Grid.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QSignalMapper>
#include <QMessageBox>
#include <QApplication>

GridEditor::GridEditor(Grid& grid, QWidget *parent)
    : QDialog(parent), _grid(grid) {
    // Setup layout and widgets
    _mainLayout = new QVBoxLayout(this);
    _gridLayout = new QGridLayout();
    _menuLayout = new QHBoxLayout();
    _resizeLayout1 = new QVBoxLayout();
    _resizeLayout2 = new QHBoxLayout();

    cellButtons.resize(_grid.getN() * _grid.getM());

    _mainLayout->addLayout(_gridLayout);
    _mainLayout->addLayout(_menuLayout);

    mapper = new QSignalMapper(this);

    
    
    for (int i = 0; i < _grid.getN(); i++) {
        for (int j = 0; j < _grid.getM(); j++) {
            QPushButton* button = new QPushButton();
            button->setFixedSize(20, 20); // Small fixed size for cell representation
            button->setCheckable(true); // Make button toggleable
            _gridLayout->addWidget(button, i, j);
            int index = i * _grid.getM() + j;
            cellButtons[index] = button; // Store button pointer in QVector

            mapper->setMapping(button, index);
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        }
    }

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(cellClicked(int)));
    
    _saveButton = new QPushButton("Save", this);
    _loadButton = new QPushButton("Load", this);
    _applyButton = new QPushButton("Apply Grid", this);
    _resizeButton = new QPushButton("Resize Grid", this);
    _gridWidthBox = new QSpinBox(this);
    _gridHeightBox = new QSpinBox(this);

    
    _gridWidthBox->setMinimum(1);
    _gridWidthBox->setMaximum(60);
    _gridHeightBox->setMinimum(1);
    _gridHeightBox->setMaximum(60);
    
    _resizeLayout2->addWidget(_gridHeightBox);
    _resizeLayout2->addWidget(_gridWidthBox);
    _resizeLayout1->addWidget(_resizeButton);

    _resizeLayout1->addLayout(_resizeLayout2);

    _menuLayout->addWidget(_saveButton);
    _menuLayout->addWidget(_loadButton);
    _menuLayout->addWidget(_applyButton);

    
    _menuLayout->addLayout(_resizeLayout1);
    
    _gridWidthBox->setRange(1, 500);  
    _gridHeightBox->setRange(1, 500);  

    _gridWidthBox->setValue(_grid.getM());
    _gridHeightBox->setValue(_grid.getN());

    connect(_resizeButton, &QPushButton::clicked, this, &GridEditor::resizeGrid);
    connect(_saveButton, &QPushButton::clicked, this, &GridEditor::saveGrid);
    connect(_loadButton, &QPushButton::clicked, this, &GridEditor::loadGrid);
    connect(_applyButton, &QPushButton::clicked, this, &GridEditor::applyButtonClick);

    updateGridUI();
}

void GridEditor::updateGridSize(){}

void GridEditor::cellClicked(int index) {
    int row = index / _grid.getM();
    int col = index % _grid.getM();
    _grid.toggleCell(row, col);  // Assuming toggleCell() toggles the state

    QPushButton* button = qobject_cast<QPushButton*>(cellButtons[index]);
    if (button) {
        button->setChecked(_grid.getCell(row, col) == 1);
        button->setStyleSheet(getStyleSheet(row, col));
    }
}

void GridEditor::updateGridUI() {
    for (int i = 0; i < _grid.getN(); i++) {
        for (int j = 0; j < _grid.getM(); j++) {
            QPushButton* button = qobject_cast<QPushButton*>(cellButtons[i * _grid.getM() + j]);
            if (button) {
                button->setChecked(_grid.getCell(i, j) == 1);
                button->setStyleSheet(getStyleSheet(i,j));
            }
        }
    }
}

void GridEditor::saveGrid() {
    // Use QFileDialog to get the file name from the user
    QString filename = QFileDialog::getSaveFileName(this, "Save Grid", "./GridLibrary", "Grid Files (*.grid)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly)) {
            // Use QDataStream for binary data writing
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_5_0);  // Set the version, matching your Qt version

            int rows = _grid.getN();
            int cols = _grid.getM();
            
            // Write rows and columns count to the file
            out << rows;
            out << cols;

            // Write grid data
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    int value = _grid.getCell(i, j);
                    out << value;
                }
            }

            file.close();
        } else {
            // Error handling if the file could not be opened
            QMessageBox::warning(this, "File Error", "Could not open the file for writing.");
        }
    }
}

void GridEditor::loadGrid() {
    QString filename = QFileDialog::getOpenFileName(this, "Load Grid", "", "Grid Files (*.grid)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_5_0);  // Ensure the version matches with what was used for writing

            // Variables to hold rows and columns count
            int rows, cols;
            in >> rows;
            in >> cols;

            // Resetting the grid with the new dimensions
            _grid.resize(rows, cols);  

            // Reading grid data
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    int value;
                    in >> value;
                    _grid.setCell(i, j, value);  
                }
            }

            file.close();

            updateGridUI();  
            QMessageBox::information(this, "Load Successful", "The grid has been loaded successfully.");

        } else {
            // Error handling if the file could not be opened
            QMessageBox::critical(this, "File Error", "Could not open the file for reading.");
        }
    }
}

void GridEditor::applyButtonClick(){
    _grid.setInitialGrid(_grid.getGridData());
    emit applyGridRequest(_grid);
}

QString GridEditor::getStyleSheet(int i, int j){
    return QString("QPushButton {"
                             "background-color: %1;"
                             "border-style: solid;"
                             "border-width: 1px;"
                             "border-color: black;"
                             "border-radius: 0px;"
                             "outline: none;}")
                     .arg(_grid.getCell(i, j) == 1 ? "black" : "white");
}
void GridEditor::resizeGrid(){
    int m = _gridWidthBox->value();
    int n = _gridHeightBox->value();
    if(n==_grid.getN() && _grid.getM()){
        QMessageBox::warning(nullptr, "Warning", "Grid size unchanged");
    }
    else if(m <= 60 && n <=60){
        _grid.resize(m,n);
        generateGrid();
    }
    else{
        QMessageBox::warning(nullptr, "Warning", "Grid size too large! Maximum is 60x60");
        _gridWidthBox->setValue(60);
        _gridHeightBox->setValue(60);
    }


    
}

void GridEditor::generateGrid(){
    //delete old grid
    QLayoutItem* item;
    while ((item = _gridLayout->takeAt(0)) != nullptr) {
        // Remove the widget from the layout and delete it
        QWidget* widget = item->widget();
        if (widget) {
            widget->deleteLater();
        }
        delete item; // Delete the layout item
    }
    cellButtons.clear();
    //create new grid
    disconnect(mapper, SIGNAL(mapped(int)), this, SLOT(cellClicked(int)));
    for (QPushButton* button : cellButtons) {
        mapper->removeMappings(button);
    }
    cellButtons.resize(_grid.getN() * _grid.getM());
    for (int i = 0; i < _grid.getN(); i++) {
        for (int j = 0; j < _grid.getM(); j++) {
            QPushButton* button = new QPushButton();
            button->setFixedSize(20, 20); // Small fixed size for cell representation
            button->setCheckable(true); // Make button toggleable
            _gridLayout->addWidget(button, i, j);
            int index = i * _grid.getM() + j;
            cellButtons[index] = button; // Store button pointer in QVector
            mapper->setMapping(button, index);
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
            //if(j%10==0){QApplication::processEvents();}
        }
    
        
    }

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(cellClicked(int)));
    
    updateGridUI();
    
    this->update();
    QApplication::processEvents();
    this->resize(minimumSize());
    
   


}