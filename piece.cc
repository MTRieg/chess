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

Piece *clonePiece(const Piece *const piece, const Board *const board) {
    return piece ? piece->clone(board) : nullptr;
}

//returns true if they have the same type and colour (or if both are null)
bool comparePieces(const Piece *const p1, const Piece *const p2){
    if(!p1 || !p2) return (p1 == p2);
    return ((p1->getType() == p2->getType()) &&
                ((p1)->getColour() == p2->getColour()));
            
}



// constructor
Piece::Piece(Colour colour, Position pos, const Board *const b) : c{colour}, pos{pos}, b{b} {}


Position::Position(string s): File{s[0]-'a'}, Rank{s[1]-'1'}{}
Position::Position(int file, int rank): File{file}, Rank{rank}{};
bool Position::inBounds(int size) const {
    return File >= 0 && File < size && Rank >= 0 && Rank < size;
}

Piece::~Piece() = default;

Colour Piece::getColour() const { return c; }
Position Piece::getPosition() const { return pos; }

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

std::string Algebraic(Position p) {
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

