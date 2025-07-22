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

char firstChar(Piece::PieceType type) {
            switch (type) {
                case Piece::PieceType::Pawn: return 'P';
                case Piece::PieceType::Knight: return 'N';
                case Piece::PieceType::Bishop: return 'B';
                case Piece::PieceType::Rook: return 'R';
                case Piece::PieceType::Queen: return 'Q';
                case Piece::PieceType::King: return 'K';
            }
            return ' ';
        }

// constructor
Piece::Piece(Colour colour, Position pos, Board *b) : c{colour}, pos{pos}, b{b} {}

Piece::~Piece() = default;

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

std::string Algebraic(Piece::Position p) {
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

