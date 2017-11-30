#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>

class QSpinBox;

class NewGameDialog : public QDialog
{
    Q_OBJECT
public:
    NewGameDialog(QWidget *parent, bool allowExit);

signals:
    void onStartGame(int size);
    void onExit();
protected Q_SLOTS:
    void onStartClicked();


private:
    QSpinBox * spinner;
};

#endif // NEWGAMEDIALOG_H
