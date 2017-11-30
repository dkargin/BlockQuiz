#include "gamedata.h"
#include <QtWidgets>
#include <cassert>

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
}

