#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>

#include "gamefield.h"

class MainWindow : public QWidget, public GameFieldController
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GameField field;

    // Generates UI for this field
    void generateField(GameField & field);

    void onFieldUpdated();
    void onBlockChanged(int x, int y);

protected:
    QGridLayout * field_layout;

    void destroyField(bool deleleWidgets=true);
};

#endif // MAINWINDOW_H
