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

class GameField
{
  public:
    GameField(GameFieldController * controller);
    // Initializes game field with given size
    void init(int width, int height);

    // Switches block state
    void switchBlock(int x, int y);

    enum BlockState
    {
        Horisontal,
        Vertical,
    };

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    typedef std::vector<BlockState> FieldState;

    void makeRandomField(FieldState & field);

    // Return to previous state
    bool rollBack();
protected:

    // A list of previous field states
    std::list<FieldState> states;
    int width = 0, height = 0;
    int retries = 0;

    GameFieldController * controller = nullptr;
};

#endif // GAMEFIELD_H
