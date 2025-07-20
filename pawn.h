#include "piece.h"
#include <vector>

class Pawn : public Piece {
    public:
        Pawn(Colour colour, Position pos, Board *b);

        Piece::PieceType getType() const override;

        bool verifyMove(Piece::Position p) override;
        std::vector<Position> validMoves() override;
};
