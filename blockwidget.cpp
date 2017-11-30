#include "blockwidget.h"
#include <sstream>
#include <QTimer>
#include <QPainter>
#include <QtWidgets>

BlockWidget::BlockWidget(GameFieldController * field, GameData * gd, int x, int y)
    :x(x), y(y), controller(field), gd(gd)
{
    state = BlockState::BlockInvalid;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
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
    QImage * img = nullptr;
    if(state == BlockState::BlockHorizontal)
        img = &gd->img_hor;
    if(state ==BlockState::BlockVertical)
        img = &gd->img_ver;

    if(img != nullptr)
        painter.drawImage(QPoint(0,0),*img);
}

//void BlockWidget::setState(BlockState new_state, int duration_ms, int delay_ms)
void BlockWidget::setState(BlockState new_state)
{
    this->state = new_state;
    // TODO: do animation stuff here
}

/*
void FieldBlock::paintEvent(QPaintEvent *)
{
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);

    int side = qMin(width(), height());
    QTime time = QTime::currentTime();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);

    painter.save();
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();

    painter.setPen(hourColor);

    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);

    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    painter.setPen(minuteColor);

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
}
*/

void BlockWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    this->controller->onBlockChanged(x, y);
}
