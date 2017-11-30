#include "gamefield.h"
#include <cassert>

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
    assert(x < width && y < height);
    int index = x + y * width;
    BlockState & s = field[index];
    if(s == BlockState::BlockHorizontal)
        s = BlockState::BlockVertical;
    else if(s == BlockState::BlockVertical)
        s = BlockState::BlockHorizontal;
    return s;
}

void GameField::setBlock(int x, int y, BlockState value)
{
    if(fields.empty())
        return;
    FieldState & field = fields.back();
    assert(x < width && y < height);
    int index = x + y * width;
    field[index] = value;
}

bool GameField::checkVictory() const
{
    if(fields.empty())
        return false;

    // Calculating a number of horizontal fields
    int completion = 0;
    for(BlockState state : fields.back())
    {
        if(state == BlockState::BlockHorizontal)
            completion++;
    }
    return completion == this->getFieldSize();
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

int GameField::getCurrentTurn() const
{
    return fields.size();
}

int GameField::getRetries() const
{
    return retries;
}
