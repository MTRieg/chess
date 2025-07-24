#pragma once
#include "player.h"

class Human : public Player {

    public:
    Human(Board* board, Colour colour);
    bool autoMovable() override;

    MoveInfo makeMove() override; // method to be implemented for human player interaction

};
