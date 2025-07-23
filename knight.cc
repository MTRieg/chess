
#include <cmath>
#include <vector>
#include "knight.h"
using namespace std;

Knight::Knight(Colour colour, Position pos, const Board *const b) : Piece(colour, pos, b) {}

Piece::PieceType Knight::getType() const {
    return PieceType::Knight;
}

Piece* Knight::clone() const {
    return new Knight(*this);
}

bool Knight::verifyMove(Position p) const {
    // Knights move in an L-shape: 2 squares in one direction and then 1 square perpendicular
    int dRank = std::abs(p.Rank - pos.Rank);
    int dFile = std::abs(p.File - pos.File);
    return (dRank == 2 && dFile == 1) || (dRank == 1 && dFile == 2);
}

std::vector<Position> Knight::validMoves() const {
    // generate all valid moves for the knight
    std::vector<Position> moves;
    for (int dr = -2; dr <= 2; ++dr) {
        for (int df = -2; df <= 2; ++df) {
            if (std::abs(dr) + std::abs(df) == 3) { // L-shape move
                Position newPos{pos.Rank + dr, pos.File + df};
                moves.push_back(newPos);
            }
        }
    }
    return moves;
}


