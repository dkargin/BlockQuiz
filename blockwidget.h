#ifndef FIELDBLOCK_H
#define FIELDBLOCK_H

#include <QWidget>

#include "gamefield.h"
#include "gamedata.h"

// UI widget to fill a block
class BlockWidget : public QWidget
{
    Q_OBJECT
public:
    BlockWidget(GameFieldController * controller, GameData * gd, int x, int y);

    QSize sizeHint() const override
    {
        return QSize(32,32);
    }

    QSize minimumSizeHint() const override
    {
        return QSize(32,32);
    }

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    void setState(BlockState state, int duration_ms, int delay_ms);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent ( QMouseEvent * event ) override;
protected:
    /// Its coordinates in game field
    int x, y;
    GameFieldController * controller;
    GameData * gd;
    // Cached state
    BlockState state;
    // Cached state
    BlockState state_transition;
};
#endif // FIELDBLOCK_H
