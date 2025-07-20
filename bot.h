#pragma once
#include "player.h"

class Bot : public Player {

    public:
    Bot(Board* board, Colour colour);
    bool autoMovable() override;
    
};
