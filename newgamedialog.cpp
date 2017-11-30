#include "newgamedialog.h"
#include <QtWidgets>

NewGameDialog::NewGameDialog(QWidget *parent, bool allow_exit)
    :QDialog(parent)
{
    QGridLayout * layout = new QGridLayout();

    QLabel * labelSize = new QLabel(tr("Field size"));
    layout->addWidget(labelSize, 0, 0, 1, 1);
    spinner = new QSpinBox();
    spinner->setRange(4, 10);
    spinner->setValue(4);
    layout->addWidget(spinner, 0, 1, 1, 1);

    QPushButton * btnDone = new QPushButton(tr("Start"));
    connect(btnDone, SIGNAL(clicked()), this, SLOT(onStartClicked()));
    layout->addWidget(btnDone, 1, 0, 1, 1 );

    if(allow_exit)
    {
        QPushButton * btnExit = new QPushButton(tr("Exit"));
        connect(btnExit, SIGNAL(clicked()), this, SIGNAL(onExit()));
        layout->addWidget(btnExit, 1, 1, 1, 1 );
    }
    else
    {
        QPushButton * btnCancel = new QPushButton(tr("Cancel"));
        connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
        layout->addWidget(btnCancel, 1, 1, 1, 1 );
    }

    this->setLayout(layout);

    this->setWindowTitle(tr("Select field size"));
}

void NewGameDialog::onStartClicked()
{
    int value = spinner->value();
    emit this->onStartGame(value);
    this->close();
}
