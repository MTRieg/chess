#include "piece.h"
#include "board.h"

Piece::Piece(Colour colour, Board *b) : c(colour), b(b) {}

Type Piece::getType() const { return type; }
Colour Piece::getColour() const { return c; }

Piece::Position Piece::getPosition() const { return pos; }

bool Piece::verifyMoves(Position p) {
    // This method should be overridden in derived classes
    return false;
}

bool Piece::move(Position p, Type promotion_type) {
    
    if (verifyMoves(p)) {
        pos = p; // Update the position if the move is valid
        return true;
    }
    return false; // Move is not valid
}

std::string Piece::Position::Algebraic(Position p) {
    // Convert the position to algebraic notation
    return std::string(1, 'a' + p.File) + std::to_string(p.Rank + 1);
}






