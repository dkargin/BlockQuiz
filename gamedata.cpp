#include "gamedata.h"
#include <QtWidgets>
#include <cassert>
#include <sstream>

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
        std::stringstream ss;
        ss<<":data/anim"<<i;
        this->hor_to_ver.push_back(QImage(ss.str().c_str()));
    }

    // Loading animation frames for rotations from 90 to 180 degrees
    // Caution: this anim sequence is hardcoded
    for(int i = 9; i < 18; i++)
    {
        std::stringstream ss;
        ss<<":data/anim"<<i;
        this->ver_to_hor.push_back(QImage(ss.str().c_str()));
    }
}

