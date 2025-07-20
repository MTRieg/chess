#include "piece.h"
#include <vector>

class Knight : public Piece {
    public:
        Knight(Colour colour, Position pos, Board *b);

        Piece::PieceType getType() const override;

        bool verifyMove(Piece::Position p) const override;
        std::vector<Position> validMoves() const override;
};
