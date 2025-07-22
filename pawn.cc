
#include <cmath>
#include <vector>
#include "pawn.h"
#include "piece.h"
#include "board.h"
using namespace std;


Pawn::Pawn(Colour colour, Position pos, Board *b) : Piece(colour, pos, b) {}

Piece::PieceType Pawn::getType() const {
    return PieceType::Pawn;
}

Piece* Pawn::clone() const {
    return new Pawn(*this);
}

bool Pawn::verifyMove(Position p) const {
    int direction = (c == Colour::White) ? 1 : -1;
    if (p.File == pos.File) {
        // Moving forward
        if (p.Rank == pos.Rank + direction) {
            return !b->pieceAtPosition(p);
        }
        // Double move from starting position
        if ((c == Colour::White && pos.Rank == 1 && p.Rank == 3) ||
            (c == Colour::Black && pos.Rank == 6 && p.Rank == 4)) {
            Position intermediate{pos.Rank + direction, pos.File};
            return !b->pieceAtPosition(p) && !b->pieceAtPosition(intermediate);
        }
    } else if (p.File == pos.File + 1 || p.File == pos.File - 1) {
        // Capturing
        if (p.Rank == pos.Rank + direction) {
            if(b->BoardInfo().enPassantFile == p.File){ 
                if(((c == Colour::White) ? 5 : 2) == p.Rank){
                    return true; //en passant
                }
            }
            return b->pieceAtPosition(p) && b->pieceAtPosition(p)->getColour() != c;
        }
        
    }
    return false;
}

std::vector<Position> Pawn::validMoves() const {
    std::vector<Position> moves;
    int direction = (c == Colour::White) ? 1 : -1;
    Position possiblePawnMoves[4] = {{pos.Rank + direction, pos.File}, {pos.Rank + direction, pos.File + 1},
                                    {pos.Rank + direction, pos.File - 1},{pos.Rank + 2 * direction, pos.File}};
    for(auto move : possiblePawnMoves){
        if(verifyMove(move)){
            moves.push_back(move);
        }
    }
    return moves;
}


