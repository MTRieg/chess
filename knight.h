#include "piece.h"

class Knight : public Piece {
    public:
        Knight(Colour colour, Board *b);

        Piece::PieceType getType() const override;

        bool verifyMoves(Piece::Position p);
        Position* validMoves() override;
};
