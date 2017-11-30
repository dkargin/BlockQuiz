#include "leaderboarddialog.h"

#include <cassert>
#include <QtWidgets>

LeaderboardDialog::LeaderboardDialog(QWidget *parent, const Leaderboard & leaderboard, bool makeNewGame)
    :QDialog(parent), newGame(makeNewGame)
{
    QGridLayout * layout = new QGridLayout();

    layout->addWidget(new QLabel(tr("Rank")), 0, 0, 1, 1);
    layout->addWidget(new QLabel(tr("Name")), 0, 1, 1, 1);
    layout->addWidget(new QLabel(tr("Turns")), 0, 2, 1, 1);
    layout->addWidget(new QLabel(tr("Retries")), 0, 3, 1, 1);

    // We will show only 10 records
    int maxRecords = 10;
    int row = 1;
    for(int i = 0; i < std::min<int>(maxRecords, leaderboard.size()); i++)
    {
        const LeaderboardRecord & rec = *leaderboard[i];

        layout->addWidget(new QLabel(QString::number(row)), row, 0, 1, 1);
        layout->addWidget(new QLabel(rec.name), row, 1, 1, 1);
        layout->addWidget(new QLabel(QString::number(rec.turns)), row, 2, 1, 1);
        layout->addWidget(new QLabel(QString::number(rec.retries)), row, 3, 1, 1);
        row++;
    }

    if(newGame)
    {
        QPushButton * btnDone = new QPushButton("New game");
        connect(btnDone, SIGNAL(clicked()), this, SLOT(close()));
        connect(btnDone, SIGNAL(clicked()), this, SIGNAL(onReturn()));
        layout->addWidget(btnDone, row, 0, 1, 1 );

        QPushButton * btnExit = new QPushButton("Exit");
        connect(btnExit, SIGNAL(clicked()), this, SLOT(close()));
        connect(btnExit, SIGNAL(clicked()), this, SIGNAL(onExit()));
        layout->addWidget(btnExit, row, 1, 1, 1 );
    }
    else
    {
        QPushButton * btnDone = new QPushButton("Return");
        connect(btnDone, SIGNAL(clicked()), this, SLOT(close()));
        layout->addWidget(btnDone, row, 0, 1, 1 );
    }

    this->setLayout(layout);
    this->setWindowTitle(tr("Leaderboard"));
    //this->setFixedSize(180, 80);
}
