#include "blockwidget.h"
#include <sstream>
#include <QTimer>
#include <QPainter>
#include <QtWidgets>

BlockWidget::BlockWidget(const GameData * gd, int x, int y)
    :x(x), y(y), gd(gd)
{
    state = BlockState::BlockInvalid;
    animated = false;
}

QSize BlockWidget::sizeHint() const
{
    int size = preferredSize();
    return QSize(size, size);
}

QSize BlockWidget::minimumSizeHint() const
{
    int size = preferredSize();
    return QSize(size, size);
}

int BlockWidget::preferredSize() const
{
    return gd != nullptr ? gd->blockSize : 1;
}

void BlockWidget::paintEvent(QPaintEvent *)
{
    QTime time = QTime::currentTime();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QImage * img = nullptr;

    if(this->animated)
    {
        int selectedFrame = frame;
        if(selectedFrame < 0)
            selectedFrame = 0;
        if(selectedFrame >= gd->animFrames)
            selectedFrame = gd->animFrames-1;

        if(state == BlockState::BlockHorizontal)
        {
            img = &gd->ver_to_hor[selectedFrame];
        }
        else if(state == BlockState::BlockVertical)
        {
            img = &gd->hor_to_ver[selectedFrame];
        }
    }
    else
    {
        if(state == BlockState::BlockHorizontal)
            img = &gd->img_hor;
        else if(state == BlockState::BlockVertical)
            img = &gd->img_ver;
    }

    if(img != nullptr)
        painter.drawImage(QPoint(0,0),*img);
}

void BlockWidget::setAnimationState(BlockState state, int delay)
{
    this->frame = -delay;
    animated = true;
}

void BlockWidget::updateAnimation(int tick)
{
    this->frame++;
    this->update();
}

//void BlockWidget::setState(BlockState new_state, int duration_ms, int delay_ms)
void BlockWidget::setState(BlockState new_state)
{
    this->state = new_state;
    // TODO: do animation stuff here
}

void BlockWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    QSize size = this->size();
    emit onClickBlock(x,y);
}
