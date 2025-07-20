#include <iostream>
#include "piece.h"
#include "board.h"

Colour &operator++(Colour &c) {
    switch (c) {
        case Colour::White: c = Colour::Black; break;
        case Colour::Black: c = Colour::White; break;
    }
    return c;
}

Piece::Piece(Colour colour, Position pos, Board *b) : c{colour}, pos{pos}, b{b} {}

Colour Piece::getColour() const { return c; }

Piece::Position Piece::getPosition() const { return pos; }

bool Piece::verifyMove(Position p) const{
    //individual pieces are encouraged to override this method, but this 
    //provides a default implementation
    for(auto &move : validMoves()) {
        if (move.Rank == p.Rank && move.File == p.File) {
            return true; 
        }
    }
    return false; 
}

bool Piece::move(Position p, PieceType promotion_type) {

    if (verifyMove(p)) {
        pos = p; // Update the position if the move is valid
        return true;
    }
    return false; // Move is not valid
}

std::string Piece::Position::Algebraic(Position p) {
    // Convert the position to algebraic notation
    return std::string(1, 'a' + p.File) + std::to_string(p.Rank + 1);
}








ostream &operator<<(ostream &out, const Colour &c) {
    switch (c) {
        case Colour::White: out << "White"; break;
        case Colour::Black: out << "Black"; break;
    }
    return out;
}

