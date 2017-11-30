#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <list>

enum class BlockState
{
    BlockInvalid,
    BlockHorizontal,
    BlockVertical,
};

typedef std::vector<BlockState> FieldState;

class GameField
{
  public:
    GameField();

    // Initializes game field with given size
    // Automatically resets current game to first turn
    void init(int width, int height);

    // Make new turn, making copy of current field
    int newTurn();
    // Return to previous state
    bool cancelTurn();
    // Get current turn
    int getCurrentTurn() const;

    int getRetries() const;

    bool valid() const;

    // Switches block state
    BlockState switchBlock(int x, int y);

    // Set block state
    void setBlock(int x, int y, BlockState value);

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    int getFieldSize() const
    {
        return width * height;
    }

    BlockState getState(int x, int y) const;

    bool checkVictory() const;

    FieldState & getCurrentField();
protected:

    // A list of previous field states
    std::list<FieldState> fields;
    int width = 0, height = 0;
    int retries = 0;
};

#endif // GAMEFIELD_H
