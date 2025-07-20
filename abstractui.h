#include "boardobserver.h"
#include "board.h"
#include "gamedata.h"

class AbstractUI : public BoardObserver {

    protected:
    Board* board;
    GameData* gameData;

    public:
    AbstractUI(Board* board, GameData* gameData);

    AbstractUI(const AbstractUI&) = delete;
    AbstractUI& operator=(const AbstractUI&) = delete;
    AbstractUI(AbstractUI&&) = delete;
    AbstractUI& operator=(AbstractUI&&) = delete;

    virtual ~AbstractUI() = default;

    virtual void update(MoveInfo latest) override = 0;
    virtual void output() = 0;

};
