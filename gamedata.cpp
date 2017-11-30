#include "gamedata.h"

GameData::GameData(QObject *parent) : QObject(parent)
{
    QResource::registerResource("data.rcc");

    this->img_hor = QImage(":data/horizontal.png");
    this->img_ver = QImage(":data/vertical.png");
}

GameData::~GameData()
{
    delete img_hor;
    delete img_ver;
}
