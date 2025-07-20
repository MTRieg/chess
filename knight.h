#include "piece.h"
#include <vector>

class Knight : public Piece {
    public:
        Knight(Colour colour, Board *b);

        Piece::PieceType getType() const override;

        bool verifyMove(Piece::Position p) override;
        std::vector<Position> validMoves() override;
};
