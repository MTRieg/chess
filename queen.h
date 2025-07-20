#include "piece.h"
#include <vector>

class Queen : public Piece {
    public:
        Queen(Colour colour, Position pos, Board *b);

        Piece::PieceType getType() const override;

        bool verifyMove(Piece::Position p) const override;
        std::vector<Position> validMoves() const override;
};
