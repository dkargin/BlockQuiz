#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QObject>
#include <vector>

class GameData : public QObject
{
    Q_OBJECT
public:
    explicit GameData(QObject *parent = nullptr);
    virtual ~GameData();

    QImage * img_hor;
    QImage * img_ver;
    // Animation track from vertical to horisontal
    std::vector<QImage*> ver_to_hor;
    // Animation track from vertical to horisontal
    std::vector<QImage*> hor_to_ver;

signals:

public slots:
};

#endif // GAMEDATA_H
