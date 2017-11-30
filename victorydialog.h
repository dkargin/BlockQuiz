#ifndef VICTORYDIALOG_H
#define VICTORYDIALOG_H

#include <QDialog>
#include "leaderboarddialog.h"  // For LeaderboardRecord

class QLineEdit;

class VictoryDialog : public QDialog
{
    Q_OBJECT
public:
    VictoryDialog(QWidget *parent, LeaderPtr rec);

protected Q_SLOTS:
    void onNextClicked();

signals:
    void onComplete(LeaderPtr rec);
protected:
    LeaderPtr rec;
    QLineEdit * editName;
};

#endif // VICTORYDIALOG_H
