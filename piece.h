#pragma once
#include <iostream>
#include <string>
#include <vector>
class Board;
using namespace std;

enum class Colour {
    White,
    Black
};

Colour &operator++(Colour &c);

class Piece {
    
    public:

    enum class PieceType {
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King
    };

    struct Position{
        int Rank, File;
        std::string Algebraic(Position p);
    };

    Piece(Colour colour, Position pos, Board *b);
    
    Colour getColour() const;
    virtual PieceType getType() const;
    Position getPosition() const;

    virtual std::vector<Position> validMoves() const = 0;
    virtual bool verifyMove(Position p) const;
    void move(Position p);

    Piece *createPiece(Colour colour, Board *b, PieceType type, Position pos);

    protected:

    PieceType type;
    Position pos;
    Colour c;
    const Board *b;

    Piece(Colour colour,  Position pos, Board *b);



};

// printing
ostream &operator<<(ostream &out, const Colour &c);

