#pragma once
#include "piece.h"
#include "board.h"

class Player {

    Board* board;
    Colour colour;

    public:
    Player(Board* board, Colour colour)
        : board(board), colour(colour) {}
    
    Colour getColour() const;

    // whether the player can move without specifying a move
    virtual bool autoMovable() = 0; 


};

