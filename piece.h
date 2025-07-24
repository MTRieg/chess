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

struct Position{
    int File, Rank;
    Position(string s);
    Position(int file, int rank);
    bool inBounds(int size) const;
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
    

    Piece(Colour colour, Position pos, const Board *const b);
    Piece(const Piece&) = default; // Copy constructor for derived classes
    Piece& operator=(const Piece&) = default; // Copy assignment operator for derived classes

    virtual ~Piece();
    
    Colour getColour() const;
    virtual PieceType getType() const = 0;
    Position getPosition() const;

    virtual std::vector<Position> validMoves() const = 0;
    virtual bool verifyMove(Position p) const;
    void move(Position p);

    virtual Piece *clone(const Board *const board = nullptr) const = 0;

    protected:

    PieceType type;
    Position pos;
    Colour c;
    const Board * const b;




};

Piece *clonePiece(const Piece *const piece, const Board *const board = nullptr);
bool comparePieces(const Piece *const p1, const Piece *const p2);

std::string Algebraic(const Position p);

// printing
ostream &operator<<(ostream &out, const Colour &c);

char firstChar(Piece::PieceType type);

