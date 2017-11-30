#include "mainwindow.h"
#include <QtWidgets>
#include "newgamedialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    fieldWidth = 0;
    fieldHeight = 0;

    gameData = new GameData();
    field_layout = new QGridLayout();
    QWidget * central = new QWidget();

    central->setLayout(field_layout);
    this->setCentralWidget(central);

    setupMenus();
    setupStatusBar();

    setWindowTitle(tr("BlockQuiz"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupMenus()
{
    QMenuBar * root = this->menuBar();
    QMenu * gameMenu = root->addMenu("Game");
    gameMenu->addAction("New", this, SLOT(showRestartGameDialog()));
    gameMenu->addAction("Cancel turn", this, SLOT(cancelTurn()));
    gameMenu->addAction("Leaderboard", this, SLOT(showLeaderboard()));
    gameMenu->addAction("Exit", this, SLOT(exitGame()));
}

void MainWindow::setupStatusBar()
{
    QStatusBar * bar = this->statusBar();

    this->labelCurrentTurn = new QLabel();
    this->labelRetries = new QLabel();
    bar->addPermanentWidget(labelCurrentTurn, 50);
    bar->addPermanentWidget(labelRetries, 50);
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
            BlockWidget * block = new BlockWidget(gameData, col, row);
            connect(block, SIGNAL(onClickBlock(int, int)), this, SLOT(onBlockChanged(int,int)));
            field_widgets.push_back(block);
            field_layout->addWidget(block, row, col, 1, 1);
        }
    }

    fieldWidth = field.getWidth();
    fieldHeight = field.getHeight();
    field_layout->update();

    this->syncUI();
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

void MainWindow::cancelTurn()
{
    this->field.cancelTurn();
    syncUI();
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

    this->labelCurrentTurn->setText(QString::asprintf("Turn: %d", field.getCurrentTurn()));
    this->labelRetries->setText(QString::asprintf("Retries: %d", field.getRetries()));
    this->statusBar()->updateGeometry();
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

void MainWindow::showNewGameDialog()
{
    NewGameDialog * dialog = new NewGameDialog(this, true);
    connect(dialog, SIGNAL(onExit()), this, SLOT(exitGame()));
    connect(dialog, SIGNAL(onStartGame(int)), this, SLOT(startNewGame(int)));
    dialog->show();
}

void MainWindow::showRestartGameDialog()
{
    NewGameDialog * dialog = new NewGameDialog(this, false);
    connect(dialog, SIGNAL(onStartGame(int)), this, SLOT(startNewGame(int)));
    dialog->show();
}

void MainWindow::showLeaderboard()
{
    // TODO: implement it
}

void MainWindow::exitGame()
{
    this->close();
}
