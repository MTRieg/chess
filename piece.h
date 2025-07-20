#include <iostream>
#include <string>
using namespace std;

class Board;
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

        virtual Position *validMoves() = 0;
        bool verifyMoves(Position p);
        bool move(Position p, PieceType promotion_type = PieceType::Queen);


    protected:
        Position pos;
        Colour c;
        const Board *b;

        Piece(Colour colour, Board *b);


};

// printing
// ostream &operator<<(ostream &out, const Colour &c) const;

