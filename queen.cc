
#include <cmath>
#include <vector>
#include "piece.h"
#include "board.h"
#include "queen.h"
using namespace std;

Queen::Queen(Colour colour, Position pos, const Board *const b) : Piece(colour, pos, b) {}

Piece::PieceType Queen::getType() const {
    return PieceType::Queen;
}

Piece* Queen::clone() const {
    return new Queen(*this);
}


bool Queen::verifyMove(Position p) const {
    if ((pos.Rank == p.Rank) && (pos.File == p.File)) {
        return false; // If the column and row are both the same, you're trying to move the piece 
                        // to the square it's already on, which is not a valid move
    } else if (pos.Rank + pos.File == p.Rank + p.File){ //diagonals such as a8 to h1
        return validMoveGivenDirection(p,1,-1) || validMoveGivenDirection(p,-1,1);
    } else if (pos.Rank - pos.File == p.Rank - p.File){ //diagonals such as a1 to h8
        return validMoveGivenDirection(p,1,1) || validMoveGivenDirection(p,-1,-1);
    } else if (pos.Rank == p.Rank){
        return validMoveGivenDirection(p,0,1) || validMoveGivenDirection(p,0,-1);
    } else if (pos.File == p.File){
        return validMoveGivenDirection(p,1,0) || validMoveGivenDirection(p,-1,0);
    }
    return false; // should never reach here
}

std::vector<Position> Queen::validMoves() const {
    std::vector<Position> moves;

    // Bishop-style movement: diagonals
    appendMovesForDirection(-1, -1, moves);
    appendMovesForDirection( 1,  1, moves);
    appendMovesForDirection(-1,  1, moves);
    appendMovesForDirection( 1, -1, moves);
    // Rook-style movement: up, down, left, right
    appendMovesForDirection(-1,  0, moves);
    appendMovesForDirection( 1,  0, moves);
    appendMovesForDirection( 0, -1, moves);
    appendMovesForDirection( 0,  1, moves);

    return moves;
}


// Helper function to get valid moves in one direction
void Queen::appendMovesForDirection(int dr, int df, std::vector<Position> &moves) const {
    int r = pos.Rank + dr, f = pos.File + df;
    while (r >= 0 && r < 8 && f >= 0 && f < 8) {
        if(!b->pieceAtSquare(f, r)){
            moves.push_back({f, r});
        }else{
            moves.push_back({f, r});
            break; // Stop after adding the first piece seen
            // Piece.cc is expected to check whether or not a move is a self-capture
        }
        r += dr;
        f += df;
    }
}

bool Queen::validMoveGivenDirection(Position p, int dr, int df) const {
    int r = pos.Rank + dr, f = pos.File + df;
    while (r >= 0 && r < 8 && f >= 0 && f < 8) {
        if (f == p.File && r == p.Rank) return true; 
        if(b->pieceAtSquare(f, r)){
            return false; // Stop if a piece is in the way
        }
        r += dr;
        f += df;
    }
    return false; //should only reach here if the target square is not reachable in the given direction
}


