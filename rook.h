#pragma once
#include "piece.h"
#include <vector>
using namespace std;

class Rook : public Piece {

    public:
    Rook(Colour colour, Position pos, Board *b);

    Piece::PieceType getType() const override;
    Piece *clone() const override;

    bool verifyMove(Position p) const override;
    vector<Position> validMoves() const override;

    private:
    void appendMovesForDirection(int dr, int df, vector<Position> &moves) const;
    bool validMoveGivenDirection(Position p, int dr, int df) const;

};
