#include "mainwindow.h"
#include <QtWidgets>
#include "newgamedialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    fieldWidth = 0;
    fieldHeight = 0;

    gameData = new GameData();
    field_layout = new QGridLayout();

    field.init(4, 4);

    generateFieldWidgets(field);

    setLayout(field_layout);
    setWindowTitle(tr("BlockQuiz"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::generateFieldWidgets(GameField & field)
{
    if(field.getWidth() != fieldWidth || field.getHeight() != fieldHeight)
    {
        destroyField();
    }

    for(int row = 0; row < field.getHeight(); row++)
    {
        for(int col = 0; col < field.getWidth(); col++)
        {
            BlockWidget * block = new BlockWidget(this, gameData, col, row);
            field_widgets.push_back(block);
            field_layout->addWidget(block, row, col, 1, 1);
        }
    }

    fieldWidth = field.getWidth();
    fieldHeight = field.getHeight();
    field_layout->update();

    this->syncUI();
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
        BlockState state = field.getState(x,y);
        block->setState(state);
        block->update();
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

void MainWindow::startNewGame(int field_size)
{
    this->field.init(field_size, field_size);
    this->generateFieldWidgets(field);
    this->show();
}

void MainWindow::showNewGameDialog(bool allowExit)
{
    NewGameDialog * dialog = new NewGameDialog(this, allowExit);
    if(allowExit)
    {
        connect(dialog, SIGNAL(onExit()), this, SLOT(exitGame()));
    }
    connect(dialog, SIGNAL(onStartGame(int)), this, SLOT(startNewGame(int)));
    dialog->show();
}

void MainWindow::exitGame()
{
    this->close();
}
