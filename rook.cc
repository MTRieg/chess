
#include <cmath>
#include <vector>
#include "piece.h"
#include "board.h"
#include "rook.h"
using namespace std;

Rook::Rook(Colour colour, Position pos, Board *b) : Piece(colour, pos, b) {}

Piece::PieceType Rook::getType() const {
    return PieceType::Rook;
}

bool Rook::verifyMove(Piece::Position p) const {
    if ((pos.Rank == p.Rank) != (pos.File == p.File)) {
        return false; // If the column and row are both different, it's not a valid rook move
                        // If the column and row are both the same, you're trying to move the piece 
                        // to the square it's already on, which is also not a valid move
    } else {
        return validMoveGivenDirection(p, 0, 1) || validMoveGivenDirection(p, 0, -1) ||
                validMoveGivenDirection(p, 1, 0) || validMoveGivenDirection(p, -1, 0);
    }

}

std::vector<Piece::Position> Rook::validMoves() const {
    std::vector<Piece::Position> moves;

    // Rook-style movement: up, down, left, right
    appendMovesForDirection(-1,  0, moves);
    appendMovesForDirection( 1,  0, moves);
    appendMovesForDirection( 0, -1, moves);
    appendMovesForDirection( 0,  1, moves);

    return moves;
}


// helpers
void Rook::appendMovesForDirection(int dr, int df, std::vector<Piece::Position> &moves) const{
    int r = pos.Rank + dr, f = pos.File + df;
    while (r >= 0 && r < 8 && f >= 0 && f < 8) {
        if(!b->pieceAtSquare(f, r)){
            moves.push_back({r, f});
        }else{
            moves.push_back({r, f});
            break; // Stop after adding the first piece seen
            // Piece.cc is expected to check whether or not a move is a self-capture
        }
        r += dr;
        f += df;
    }
}

bool Rook::validMoveGivenDirection(Piece::Position p, int dr, int df) const {
    int r = pos.Rank + dr, f = pos.File + df;
    while (r >= 0 && r < 8 && f >= 0 && f < 8) {
        if(b->pieceAtSquare(f, r)){
            Colour targetColour = b->pieceAtSquare(f, r)->getColour();
            if (targetColour == c) return false; // Stop if own piece
            if (f == p.File && r == p.Rank) return true; // return true if the target square is reached
            return false; // Stop if opponent's piece
        }
        r += dr;
        f += df;
    }
    return false; //should only reach here if the target square is not reachable in the given direction
}


