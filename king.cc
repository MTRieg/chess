
#include <cmath>
#include <vector>
#include "piece.h"
#include "king.h"
using namespace std;


King::King(Colour colour, Position pos, Board *b) : Piece(colour, pos, b) {}

Piece::PieceType King::getType() const {
    return Piece::PieceType::King;
}

Piece* King::clone() const {
    return new King(*this);
}

bool King::verifyMove(Position p) const {
    // kings move one square in any direction
    int dRank = std::abs(p.Rank - pos.Rank);
    int dFile = std::abs(p.File - pos.File);
    return (dRank <= 1 && dFile <= 1);
}

std::vector<Position> King::validMoves() const {
    // generate all valid moves for the king
    std::vector<Position> moves;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int df = -1; df <= 1; ++df) {
            if (dr != 0 || df != 0) { // exclude the current position
                Position newPos{pos.Rank + dr, pos.File + df};
                moves.push_back(newPos);
            }
        }
    }
    return moves;
}







