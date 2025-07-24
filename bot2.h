#pragma once
#include "bot.h"

class Bot2 : public Bot {
    
    public:
    Bot2(Board* board, Colour colour);
    MoveInfo &makeMove() override;
    
};
