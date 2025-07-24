#pragma once
#include "bot.h"

class Bot1 : public Bot {
    
    public:
    Bot1(Board* board, Colour colour);
    MoveInfo &makeMove() override; 
    
};
