#include "player.h"

class Human : public Player {

    public:
    Human(Board* board, Colour colour);
    bool autoMovable() override;

};
