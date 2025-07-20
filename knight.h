#pragma once
#include "piece.h"
#include <vector>
using namespace std;

class Knight : public Piece {
    public:
        Knight(Colour colour, Position pos, Board *b);

        Piece::PieceType getType() const override;

        bool verifyMove(Piece::Position p) const override;
        vector<Position> validMoves() const override;
};
