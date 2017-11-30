#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QObject>
#include <QImage>
#include <vector>


// Helper class to initialize and clear resources
class ResourceWrapper
{
    std::string path;
public:
    ResourceWrapper(const char * path);
    ~ResourceWrapper();
};

// Contains references to game resources
class GameData
{
    ResourceWrapper res_helper;
public:
    GameData();

    QImage img_hor;
    QImage img_ver;

    // Number of animation frames
    int animFrames;
    // Desired block size, in pixels
    int blockSize;
    // Animation track from vertical to horisontal
    std::vector<QImage> ver_to_hor;
    // Animation track from vertical to horisontal
    std::vector<QImage> hor_to_ver;
};

#endif // GAMEDATA_H
