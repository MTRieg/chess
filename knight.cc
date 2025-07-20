
#include "piece.h"
#include <cmath>
#include <vector>




class Knight: public Piece{
    public:
        Knight(Colour colour, Position pos, Board *b) : Piece(colour, pos, b) {}

        PieceType getType() const override {
            return PieceType::Knight;
        }

        bool verifyMove(Position p) override{
            // Knights move in an L-shape: 2 squares in one direction and then 1 square perpendicular
            int dRank = std::abs(p.Rank - pos.Rank);
            int dFile = std::abs(p.File - pos.File);
            return (dRank == 2 && dFile == 1) || (dRank == 1 && dFile == 2);
        }

        std::vector<Position> validMoves() override {
            // Generate all valid moves for the knight
            std::vector<Position> moves;
            for (int dr = -2; dr <= 2; ++dr) {
                for (int df = -2; df <= 2; ++df) {
                    if (std::abs(dr) + std::abs(df) == 3) { // L-shape move
                        Position newPos{pos.Rank + dr, pos.File + df};
                        moves.push_back(newPos);
                    }
                }
            }
            return moves;
        }
};






