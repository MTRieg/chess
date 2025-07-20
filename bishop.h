#pragma once
#include "piece.h"
#include <vector>
using namespace std;

class Bishop : public Piece {

    public:

    Bishop(Colour colour, Position pos, Board *b);

    Piece::PieceType getType() const override;
    Piece *clone() const override;

    bool verifyMove(Piece::Position p) const override;
    vector<Piece::Position> validMoves() const override;

    private:

    void appendMovesForDirection(int dr, int df, vector<Position> &moves) const;
    bool validMoveGivenDirection(Piece::Position p, int dr, int df) const;

};
