
#include <cmath>
#include <vector>
#include "piece.h"
#include "king.h"
using namespace std;


King::King(Colour colour, Position pos, const Board *const b) : Piece(colour, pos, b) {}

Piece::PieceType King::getType() const {
    return Piece::PieceType::King;
}

Piece* King::clone(const Board *const board) const {
    if (board) return new King(this->c, this->pos, board);
    return new King(*this);
}

bool King::verifyMove(Position p) const {
    // kings move one square in any direction
    int dRank = std::abs(p.Rank - pos.Rank);
    int dFile = std::abs(p.File - pos.File);
    if(dFile == 2 && dRank == 0) return(pos.File == 4 && (pos.Rank == 0 || pos.Rank == 7)); // Castling move
    return (dRank <= 1 && dFile <= 1);
}

std::vector<Position> King::validMoves() const {
    // generate all valid moves for the king
    std::vector<Position> moves;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int df = -1; df <= 1; ++df) {
            if (dr != 0 || df != 0) { // exclude the current position
                Position newPos{pos.File + df, pos.Rank + dr};
                moves.push_back(newPos);
            }
        }
    }
    if(pos.File == 4 && (pos.Rank == 0 || pos.Rank == 7)){
        Position kingside = Position{6, pos.Rank};
        Position queenside = Position{2, pos.Rank};
        moves.push_back(kingside);
        moves.push_back(queenside);
    }

    return moves;
}







