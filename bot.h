#pragma once
#include "player.h"

class Bot : public Player {

    public:
    Bot(Board* board, Colour colour);
    virtual ~Bot() = default; // optional, but recommended
    bool autoMovable() override;

    virtual MoveInfo makeMove() = 0;

    
};