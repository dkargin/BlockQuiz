#include "gamefield.h"

GameField::GameField(GameFieldController * controller)
    : controller(controller)
{

}

void GameField::init(int width, int height)
{
    this->width = width;
    this->height = height;
}

void GameField::makeRandomField(FieldState &field)
{
    for(int i = 0; i < field.size(); i++)
    {
        field[i] = (rand() & 0x1) ? Horisontal : Vertical;
    }
}


