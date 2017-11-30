#include "mainwindow.h"
#include <sstream>

#include <QtWidgets>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), field(this)
{
    this->gameData = new GameData(this);
    this->field.init(4, 4);

    field_layout = new QGridLayout();

    generateField(field);
    setLayout(field_layout);

    setWindowTitle(tr("BlockQuiz"));
}



MainWindow::~MainWindow()
{

}

void MainWindow::generateField(GameField & field)
{
    destroyField();
    for(int row = 0; row < field.getHeight(); row++)
    {
        for(int col = 0; col < field.getWidth(); col++)
        {
            BlockWidget * block = new BlockWidget(this, gameData, col, row);
            field_layout->addWidget(block, row, col, 1, 1);
        }
    }
}

void MainWindow::onFieldUpdated()
{

}

void MainWindow::onBlockChanged(int x, int y)
{
    // 1. Generate new field state
    field.newTurn();
    // 2. Update columns and rows
    for(int col = 0; col < field.getWidth(); col++)
        field.switchBlock(col, y);

    for(int row = 0; row < field.getWidth(); row++)
        if(row != y)    // skipping central block.
            field.switchBlock(x, row);
    // 3. Sync UI according to new field state
    this->syncUI();
}

void MainWindow::syncUI()
{
    for(BlockWidget * block: field_widgets)
    {
        int x = block->getX();
        int y = block->getY();
        block->setState(field.getState(x,y), 0, 0);
    }
}

void clearLayout(QLayout * layout, bool deleleWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleleWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleleWidgets);
        delete item;
    }
}


void MainWindow::destroyField(bool deleleWidgets)
{
    field_widgets.clear();
    clearLayout(field_layout, deleleWidgets);
}
