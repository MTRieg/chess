#include <iostream>
#include <string>
#include <vector>

#ifndef BOARD_H
#define BOARD_H
class Board;
#endif 

struct Position;
struct Colour;

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

        enum class Colour {
            White,
            Black
        };

        struct Position{
            int Rank, File;
            std::string Algebraic(Position p);
        };

        

        virtual PieceType getType() const;
        Colour getColour() const;
        Position getPosition() const;

        virtual std::vector<Position> validMoves() = 0;
        virtual bool verifyMove(Position p);
        bool move(Position p, PieceType promotion_type = PieceType::Queen);


    protected:
        Position pos;
        Colour c;
        const Board *b;

        Piece(Colour colour, Board *b);


};

// printing
// ostream &operator<<(ostream &out, const Colour &c) const;

