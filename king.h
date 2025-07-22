#pragma once
#include "piece.h"
#include <vector>
using namespace std;

class King : public Piece {

    public:

    King(Colour colour, Position pos, Board *b);

    Piece::PieceType getType() const override;
    Piece *clone() const override;

    bool verifyMove(Position p) const override;
    vector<Position> validMoves() const override;

};
