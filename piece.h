#include <string>


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

        Piece(PieceType type, Colour colour, Board *b);

        virtual PieceType getType() const;
        Colour getColour() const;
        Position getPosition() const;

        virtual Position *validMoves();
        bool verifyMoves(Position p);
        bool move(Position p, PieceType promotion_type = PieceType::Queen);


    protected:
        Position pos;
        Colour c;
        const Board *v;


};

