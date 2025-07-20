
#include "piece.h"
#include <cmath>
#include <vector>




class King: public Piece{
    public:
        King(Colour colour, Board *b) : Piece(colour, b) {}

        PieceType getType() const override {
            return PieceType::King;
        }

        bool verifyMove(Position p) override{
            // Kings move one square in any direction
            int dRank = std::abs(p.Rank - pos.Rank);
            int dFile = std::abs(p.File - pos.File);
            return (dRank <= 1 && dFile <= 1);
        }

        std::vector<Position> validMoves() override {
            // Generate all valid moves for the king
            std::vector<Position> moves;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int df = -1; df <= 1; ++df) {
                    if (dr != 0 || df != 0) { // Exclude the current position
                        Position newPos{pos.Rank + dr, pos.File + df};
                        moves.push_back(newPos);
                    }
                }
            }
            return moves;
        }
};






