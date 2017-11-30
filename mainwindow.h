#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "blockwidget.h"
#include "gamefield.h"
#include "gamedata.h"

class MainWindow : public QWidget, public GameFieldController
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GameField field;

    // Generates UI for this field
    void generateFieldWidgets(GameField & field);

    void onFieldUpdated();
    void onBlockChanged(int x, int y);

    void syncUI();
    // Start new game and show its dialog
    void showNewGameDialog(bool allowExit);
protected Q_SLOTS:
    void startNewGame(int size);
    void exitGame();
protected:
    QGridLayout * field_layout;

    GameData * gameData;

    std::vector<BlockWidget*> field_widgets;

    int fieldWidth, fieldHeight;
    void loadData();
    void destroyField(bool deleleWidgets=true);
};

#endif // MAINWINDOW_H
