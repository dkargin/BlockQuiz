#include "mainwindow.h"
#include <sstream>

#include <QtWidgets>

FieldBlock::FieldBlock(GameFieldController * field, int x, int y)
    :QLabel(), x(x), y(y), controller(field)
{
    std::stringstream ss;
    ss << x<<":"<<y;
    this->setText(ss.str().c_str());
    this->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

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


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), field(this)
{
    this->field.init(4, 4);

    field_layout = new QGridLayout();

    generateField(field);
    setLayout(field_layout);

    setWindowTitle(tr("BlockQuiz"));
}



MainWindow::~MainWindow()
{

}

void MainWindow::generateField(GameField & field)
{
    destroyField();
    for(int row = 0; row < field.getHeight(); row++)
    {
        for(int col = 0; col < field.getWidth(); col++)
        {
            FieldBlock * block = new FieldBlock(this, col, row);
            field_layout->addWidget(block, row, col, 1, 1);
        }
    }
}

void MainWindow::onFieldUpdated()
{

}

void MainWindow::onBlockChanged(int x, int y)
{

}

void clearLayout(QLayout * layout, bool deleleWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleleWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleleWidgets);
        delete item;
    }
}


void MainWindow::destroyField(bool deleleWidgets)
{
    clearLayout(field_layout, deleleWidgets);
}
