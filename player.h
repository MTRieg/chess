#include "piece.h"
#include "board.h"

class Player {

    Board* board;
    Piece::Colour colour;

    public:
    Player(Board* board, Piece::Colour colour)
        : board(board), colour(colour) {}

};

