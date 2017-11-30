#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "blockwidget.h"
#include "gamefield.h"
#include "gamedata.h"

#include "newgamedialog.h"
#include "leaderboarddialog.h"
#include "victorydialog.h"

class QLabel;
class QGridLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GameField field;

    void readLeaderboard();
    void writeLeaderboard();
    void setupMenus();
    void setupStatusBar();
    // Generates UI for this field
    void generateFieldWidgets(GameField & field);
    void syncUI();
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

public Q_SLOTS:
    // Start new game and show its dialog
    void showNewGameDialog();
protected Q_SLOTS:
    LeaderPtr addLeaderboardRecord(int turns, int retries);
    void showRestartGameDialog();

    void showLeaderboard();
    void cancelTurn();
    void startNewGame(int size);
    void exitGame();
    void onBlockChanged(int x, int y);

    void onCompleteGame(LeaderPtr rec); // Called when player has entered his name to leaderboard
protected:
    QGridLayout * field_layout;
    QLabel * labelCurrentTurn;
    QLabel * labelRetries;

    GameData * gameData;

    // We keep array of block widgets for fast ui synchronisation
    std::vector<BlockWidget*> field_widgets;
    Leaderboard leaderboard;

    int fieldWidth, fieldHeight;
    QString getSettingsPath();
    QString lastName;           // Last player name
    void loadData();
    void destroyField(bool deleleWidgets=true);
};

#endif // MAINWINDOW_H
