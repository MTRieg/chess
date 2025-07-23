#include <vector>
#include <cmath>
#include "piece.h"
#include "board.h"
#include "bishop.h"
using namespace std;

// constructor
Bishop::Bishop(Colour colour, Position pos, const Board *const b) : Piece(colour, pos, b) {}

Piece::PieceType Bishop::getType() const {
    return PieceType::Bishop;
}
    
Piece* Bishop::clone() const {
    return new Bishop(*this);
}


bool Bishop::verifyMove(Position p) const {
    if(((pos.Rank == p.Rank) && (pos.File == p.File))) {
        return false; // If the column and row are both the same, you're trying to move the piece 
                        // to the square it's already on, which is not a valid move
    }else if(pos.Rank + pos.File == p.Rank + p.File){ //diagonals such as a8 to h1
        return validMoveGivenDirection(p,1,-1) || validMoveGivenDirection(p,-1,1);
    }else if(pos.Rank - pos.File == p.Rank - p.File){ //diagonals such as a1 to h8
        return validMoveGivenDirection(p,1,1) || validMoveGivenDirection(p,-1,-1);
    }
    return false; //if the move is not diagonal, it's not a valid bishop move

}

vector<Position> Bishop::validMoves() const {
    vector<Position> moves;

    // bishop-style movement on diagonals
    appendMovesForDirection(-1, -1, moves);
    appendMovesForDirection( 1,  1, moves);
    appendMovesForDirection(-1,  1, moves);
    appendMovesForDirection( 1, -1, moves);

    return moves;
}

// helpers
void Bishop::appendMovesForDirection(int dr, int df, vector<Position> &moves) const {
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

bool Bishop::validMoveGivenDirection(Position p, int dr, int df) const {
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

