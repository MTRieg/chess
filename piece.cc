#include <iostream>
#include "piece.h"
#include "board.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

// colour iterator
Colour &operator++(Colour &c) {
    switch (c) {
        case Colour::White: c = Colour::Black; break;
        case Colour::Black: c = Colour::White; break;
    }
    return c;
}

// constructor
Piece::Piece(Colour colour, Position pos, Board *b) : c{colour}, pos{pos}, b{b} {}

Piece *Piece::CreatePiece(Colour colour, Board *b, Piece::PieceType type, Piece::Position pos) {
    switch (type) {
        case Piece::PieceType::Pawn: return new Pawn(colour, pos, b);
        case Piece::PieceType::Knight: return new Knight(colour, pos, b);
        case Piece::PieceType::Bishop: return new Bishop(colour, pos, b);
        case Piece::PieceType::Rook: return new Rook(colour, pos, b);
        case Piece::PieceType::Queen: return new Queen(colour, pos, b);
        case Piece::PieceType::King: return new King(colour, pos, b);
        default: return nullptr;
    }
}

// accessors
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

void Piece::move(Position p) {pos = p;}

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

