#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <list>

class GameFieldController
{
public:
    virtual void onFieldUpdated() = 0;
    virtual void onBlockChanged(int x, int y) = 0;
    virtual ~GameFieldController() {}
};

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
    GameField(GameFieldController * controller);

    // Initializes game field with given size
    void init(int width, int height);

    // Make new turn, making copy of current field
    int newTurn();
    // Return to previous state
    bool cancelTurn();
    // Get current turn
    int currentTurn() const;

    // Switches block state
    BlockState switchBlock(int x, int y);

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

    //
    FieldState & getCurrentField();
protected:

    // A list of previous field states
    std::list<FieldState> fields;
    int width = 0, height = 0;
    int retries = 0;

    GameFieldController * controller = nullptr;
};

#endif // GAMEFIELD_H
