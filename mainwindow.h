#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "blockwidget.h"
#include "gamefield.h"
#include "gamedata.h"

class QLabel;
class QGridLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GameField field;

    void setupMenus();
    void setupStatusBar();
    // Generates UI for this field
    void generateFieldWidgets(GameField & field);
    void syncUI();

    // Start new game and show its dialog
    void showNewGameDialog();
protected Q_SLOTS:
    void showRestartGameDialog();
    void showLeaderboard();
    void cancelTurn();
    void startNewGame(int size);
    void exitGame();
    void onBlockChanged(int x, int y);
protected:
    QGridLayout * field_layout;
    QLabel * labelCurrentTurn;
    QLabel * labelRetries;

    GameData * gameData;

    std::vector<BlockWidget*> field_widgets;

    int fieldWidth, fieldHeight;
    void loadData();
    void destroyField(bool deleleWidgets=true);
};

#endif // MAINWINDOW_H
