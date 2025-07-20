#include "piece.h"
#include <vector>

class Bishop : public Piece {
    public:
        Bishop(Colour colour, Board *b);

        Piece::PieceType getType() const override;

        bool verifyMove(Piece::Position p) override;
        std::vector<Position> validMoves() override;
};
