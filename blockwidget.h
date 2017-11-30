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
    BlockWidget(const GameData * gd, int x, int y);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    int preferredSize() const;

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    void setState(BlockState state);
    void setAnimationState(BlockState state, int delay);
    void updateAnimation(int tick);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent ( QMouseEvent * event ) override;
signals:
    void onClickBlock(int x, int y);
protected:
    /// Its coordinates in game field
    int x, y;
    const GameData * gd;
    // Cached state
    BlockState state;
    // Cached state
    BlockState state_transition;

    bool animated;
    int frame;      // Animation frame
};
#endif // FIELDBLOCK_H
