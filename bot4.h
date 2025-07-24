#pragma once
#include "bot.h"

class Bot4 : public Bot {
    
    public:
    Bot4(Board* board, Colour colour);
    MoveInfo makeMove() override;
    
};
