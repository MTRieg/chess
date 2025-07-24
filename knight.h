#pragma once
#include "piece.h"
#include <vector>
using namespace std;

class Knight : public Piece {

    public:

    Knight(Colour colour, Position pos, const Board *const b);

    Piece::PieceType getType() const override;
    Piece *clone(const Board *const board) const override;

    bool verifyMove(Position p) const override;
    vector<Position> validMoves() const override;
};
