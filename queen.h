#include "piece.h"
#include <vector>

class Queen : public Piece {
    public:
        Queen(Colour colour, Board *b);

        Piece::PieceType getType() const override;

        bool verifyMove(Piece::Position p) override;
        std::vector<Position> validMoves() override;
};
