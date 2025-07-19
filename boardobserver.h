#include "gamedata.h"

class BoardObserver {

    public:
    virtual ~BoardObserver() = 0;
    virtual void update(MoveInfo latest) = 0;
}