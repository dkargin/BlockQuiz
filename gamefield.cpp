#include "gamefield.h"

void makeRandomField(FieldState &field)
{
    for(BlockState & state : field)
    {
        state = (rand() & 0x1) ? BlockState::BlockHorizontal : BlockState::BlockVertical;
    }
}

GameField::GameField()
{

}

void GameField::init(int width, int height)
{
    // We reset all data even if geometry is the same
    this->width = width;
    this->height = height;
    this->retries = 0;

    // Generate new random state
    FieldState state(this->getFieldSize());
    makeRandomField(state);
    this->fields.clear();
    fields.push_back(std::move(state));
}

bool GameField::valid() const
{
    return width != 0 && height != 0;
}

int GameField::newTurn()
{
    FieldState field = fields.back();
    fields.push_back(std::move(field));
    return fields.size();
}

bool GameField::cancelTurn()
{
    if(fields.size() > 1)
    {
        fields.pop_back();
        this->retries++;
        return true;
    }
    return false;
}

BlockState GameField::switchBlock(int x, int y)
{
    if(fields.empty())
        return BlockState::BlockInvalid;
    FieldState & field = fields.back();
    int index = x + y * width;
    BlockState & s = field[index];
    if(s == BlockState::BlockHorizontal)
        s = BlockState::BlockVertical;
    else if(s == BlockState::BlockVertical)
        s = BlockState::BlockHorizontal;
    return s;
}

BlockState GameField::getState(int x, int y) const
{
    if(!fields.empty())
    {
        int index = x + y * width;
        return fields.back()[index];
    }
    return BlockState::BlockInvalid;
}
