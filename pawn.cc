
#include "piece.h"
#include <cmath>
#include <vector>
#include "board.h"




class Pawn: public Piece{
    public:
        Pawn(Colour colour, Board *b) : Piece(colour, b) {}

        PieceType getType() const override {
            return PieceType::Pawn;
        }

        bool verifyMove(Position p) override{
        }

        std::vector<Position> validMoves() override {
            std::vector<Position> moves;
            int direction = (c == Colour::White) ? 1 : -1;
            Position oneSquareForward{pos.Rank + direction, pos.File};
            if (!b->pieceAtPosition(oneSquareForward)) {
                moves.push_back(oneSquareForward);
                // If on starting rank, can move two squares forward
                if ((c == Colour::White && pos.Rank == 1) || (c == Colour::Black && pos.Rank == 6)) {
                    Position twoSquaresForward{pos.Rank + 2 * direction, pos.File};
                    if (!b->pieceAtPosition(twoSquaresForward)) {
                        moves.push_back(twoSquaresForward);
                    }
                }
            }
            // Capture diagonally
            for (int df = -1; df <= 1; df += 2) {
                Position diagonalCapture{pos.Rank + direction, pos.File + df};
                if (b->pieceAtPosition(diagonalCapture) && b->pieceAtPosition(diagonalCapture)->getColour() != c) {
                    moves.push_back(diagonalCapture);
                }
            }
            return moves;
        }
};






