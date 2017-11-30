#include "victorydialog.h"

#include <QtWidgets>

VictoryDialog::VictoryDialog(QWidget *parent, LeaderPtr rec)
    :QDialog(parent, 0), rec(rec)
{
    QGridLayout * layout = new QGridLayout();

    QLabel * labelSize = new QLabel(tr("Name"));
    layout->addWidget(labelSize, 0, 0, 1, 1);

    editName = new QLineEdit();

    layout->addWidget(editName, 0, 1, 1, 1);

    QPushButton * btnDone = new QPushButton(tr("Next"));

    connect(btnDone, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnDone, SIGNAL(clicked()), this, SLOT(onNextClicked()));

    layout->addWidget(btnDone, 1, 0, 1, 2);
    this->setLayout(layout);

    this->setWindowTitle(QString::asprintf("Victory! Your rank is %d", rec->rank+1));

    //this->setFixedSize(180, 80);
}

void VictoryDialog::onNextClicked()
{
    rec->name = editName->text();
    emit onComplete(rec);
    this->hide();
}
