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

    bool isLocked() const;
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

public Q_SLOTS:
    // Start new game and show its dialog
    void showNewGameDialog();
protected Q_SLOTS:
    LeaderPtr addLeaderboardRecord(int turns, int retries);
    void showRestartGameDialog();
    void updateAnimation();
    void showLeaderboard();
    void cancelTurn();
    void startNewGame(int size);
    void exitGame();
    void onBlockChanged(int x, int y);

    void onCompleteGame(LeaderPtr rec); // Called when player has entered his name to leaderboard
    void releaseDialogLock();           // Release lock of game board because of active dialog window
protected:
    QString getSettingsPath();
    // Wrapper to show dialog windows. Manages locking of game board
    void startDialog(QDialog * dialog);
    bool hasEnqueuedCommand() const;
    void runNextCommand();
protected:
    int dialogLocked = 0;               // Counter for dialog locks
    int animationTick = 0;              // Current animation tick
    int animationDuration = 0;          // Number of ticks for current animation transform
    // Coordinates of queued block change
    int queuedBlockX = -1, queuedBlockY = -1;

    QGridLayout * field_layout;
    QLabel * labelCurrentTurn;
    QLabel * labelRetries;
    QTimer * animTimer;

    GameData * gameData;

    // We keep array of block widgets for fast ui synchronisation
    std::vector<BlockWidget*> field_widgets;
    Leaderboard leaderboard;

    int fieldWidth, fieldHeight;

    QString lastName;           // Last player name
};

#endif // MAINWINDOW_H
