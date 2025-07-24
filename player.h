#pragma once
#include "piece.h"
#include "board.h"

class Player {

    protected:
    Board* board;
    Colour colour;

    public:
    Player(Board* board, Colour colour)
        : board(board), colour(colour) {}
    
    Colour getColour() const;

    virtual ~Player() = default;
    virtual MoveInfo &makeMove() = 0; // method to be implemented by derived classes for making a move

    // whether the player can move without specifying a move
    virtual bool autoMovable() = 0; 


};

