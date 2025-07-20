
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
            return b->pieceAtPosition(p) && b->pieceAtPosition(p)->getColour() != c;
        }
    }
    return false;
}

std::vector<Piece::Position> Pawn::validMoves() const {
    std::vector<Piece::Position> moves;
    int direction = (c == Colour::White) ? 1 : -1;
    Piece::Position oneSquareForward{pos.Rank + direction, pos.File};
    if (!b->pieceAtPosition(oneSquareForward)) {
        moves.push_back(oneSquareForward);
        // If on starting rank, can move two squares forward
        if ((c == Colour::White && pos.Rank == 1) || (c == Colour::Black && pos.Rank == 6)) {
            Position twoSquaresForward{pos.Rank + 2 * direction, pos.File};
            if (!b->pieceAtPosition(twoSquaresForward)) {
                moves.push_back(twoSquaresForward);
            }
        }
    }
    // Capture diagonally
    for (int df = -1; df <= 1; df += 2) {
        Piece::Position diagonalCapture{pos.Rank + direction, pos.File + df};
        if (b->pieceAtPosition(diagonalCapture) && b->pieceAtPosition(diagonalCapture)->getColour() != c) {
            moves.push_back(diagonalCapture);
        }
    }
    return moves;
}


