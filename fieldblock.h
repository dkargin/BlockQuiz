#ifndef FIELDBLOCK_H
#define FIELDBLOCK_H

#include <QWidget>

#include "gamefield.h"

// UI widget to fill a block
class FieldBlock : public QWidget
{
    Q_OBJECT
public:
    FieldBlock(GameFieldController * controller, int x, int y);

    QSize sizeHint() const override
    {
        return QSize(32,32);
    }
    QSize minimumSizeHint() const override
    {
        return QSize(32,32);
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent ( QMouseEvent * event ) override;
protected:
    /// Its coordinates in game field
    int x, y;
    GameFieldController * controller;
};
#endif // FIELDBLOCK_H
