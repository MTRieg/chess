
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


