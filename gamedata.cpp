#include "gamedata.h"
#include <QtWidgets>
#include <cassert>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(logDebug)

Q_LOGGING_CATEGORY(logDebug,    "Debug")

ResourceWrapper::ResourceWrapper(const char * path)
    :path(path)
{
    QResource::registerResource(this->path.c_str());
}

ResourceWrapper::~ResourceWrapper()
{
    QResource::unregisterResource(this->path.c_str());
}

GameData::GameData()
    :res_helper("data.rcc")
    ,img_hor(":data/horizontal.png")
    ,img_ver(":data/vertical.png")
{
    assert(!img_hor.isNull() && !img_ver.isNull());

    this->blockSize = img_hor.width();

    // Loading animation frames for rotations from 0 to 90 degrees
    // Caution: this anim frames are hardcoded
    for(int i = 0; i < 9; i++)
    {
        QString path = QString::asprintf(":data/anim%d.png", i);
        this->hor_to_ver.push_back(QImage(path));
        if(hor_to_ver.back().isNull())
        {
            qDebug().nospace() << "Failed to load file: " << path;
        }
    }

    // Loading animation frames for rotations from 90 to 180 degrees
    // Caution: this anim sequence is hardcoded
    for(int i = 9; i < 18; i++)
    {
        QString path = QString::asprintf(":data/anim%d.png", i);
        this->ver_to_hor.push_back(QImage(path));
        if(ver_to_hor.back().isNull())
        {
            qDebug().nospace() << "Failed to load file: " << path;
        }
    }
}

