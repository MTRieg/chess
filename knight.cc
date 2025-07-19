
#include "piece.h"
#include "board.h"
#include <cmath>




class Knight: public Piece{
    public:
        Knight(Colour colour, Board *b) : Piece(colour, b) {}

        Type getType() const override {
            return Type::Knight;
        }

        bool verifyMoves(Position p) override {
            // Knights move in an L-shape: 2 squares in one direction and then 1 square perpendicular
            int dRank = std::abs(p.Rank - pos.Rank);
            int dFile = std::abs(p.File - pos.File);
            return (dRank == 2 && dFile == 1) || (dRank == 1 && dFile == 2);
        }

        Position *validMoves() override {
            // Generate all valid moves for the knight
            static Position moves[8];
            int index = 0;

            for (int dr = -2; dr <= 2; ++dr) {
                for (int df = -2; df <= 2; ++df) {
                    if (std::abs(dr) + std::abs(df) == 3) { // L-shape move
                        Position newPos(pos.Rank + dr, pos.File + df);
                        if (b->isValidPosition(newPos)) {
                            moves[index++] = newPos;
                        }
                    }
                }
            }
            return moves;
        }
};






