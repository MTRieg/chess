#pragma once
#include "piece.h"
#include <vector>
using namespace std;

class Queen : public Piece {
    public:
    Queen(Colour colour, Position pos, const Board *const b);

    Piece::PieceType getType() const override;
    Piece *clone(const Board *const board) const override;

    bool verifyMove(Position p) const override;
    vector<Position> validMoves() const override;

    private:
    void appendMovesForDirection(int dr, int df, vector<Position> &moves) const;
    bool validMoveGivenDirection(Position p, int dr, int df) const;

};
