
#include "piece.h"
#include "board.h"
#include <cmath>
#include <vector>




class Queen: public Piece{
    public:
        Queen(Colour colour, Board *b) : Piece(colour, b) {}

        PieceType getType() const override {
            return PieceType::Queen;
        }

        bool verifyMove(Position p) override{
            if(((pos.Rank == p.Rank) && (pos.File == p.File))) {
                return false; // If the column and row are both the same, you're trying to move the piece 
                              // to the square it's already on, which is not a valid move
            }else if(pos.Rank + pos.File == p.Rank + p.File){ //diagonals such as a8 to h1
                return validMoveGivenDirection(p,1,-1) || validMoveGivenDirection(p,-1,1);
            }else if(pos.Rank - pos.File == p.Rank - p.File){ //diagonals such as a1 to h8
                return validMoveGivenDirection(p,1,1) || validMoveGivenDirection(p,-1,-1);
            }else if(pos.Rank == p.Rank){
                return validMoveGivenDirection(p,0,1) || validMoveGivenDirection(p,0,-1);
            }else if(pos.File == p.File){
                return validMoveGivenDirection(p,1,0) || validMoveGivenDirection(p,-1,0);
            }
            return false; // Should never reach here



        }

        std::vector<Position> validMoves() override {
            std::vector<Position> moves;

            

            // Bishop-style movement: diagonals
            appendMovesForDirection(-1,  -1, moves);
            appendMovesForDirection( 1,  1, moves);
            appendMovesForDirection(-1,   1, moves);
            appendMovesForDirection( 1,  -1, moves);
            // Rook-style movement: up, down, left, right
            appendMovesForDirection(-1,  0, moves);
            appendMovesForDirection( 1,  0, moves);
            appendMovesForDirection( 0, -1, moves);
            appendMovesForDirection( 0,  1, moves);

            return moves;
        }







    private:
        // Helper function to get valid moves in one direction
        void appendMovesForDirection(int dr, int df, std::vector<Position> moves = {}) {
            int r = pos.Rank + dr, f = pos.File + df;
            while (r >= 0 && r < 8 && f >= 0 && f < 8) {
                if(!b->pieceAtSquare(f, r)){
                    moves.push_back({r, f});
                }else{
                    Colour targetColour = b->pieceAtSquare(f, r)->getColour();
                    if (targetColour == c) break; // Stop if own piece
                    moves.push_back({r, f});
                    break; // Stop if opponent's piece
                }
                r += dr;
                f += df;
            }
        }

        bool validMoveGivenDirection(Position p, int dr, int df) const {
            int r = pos.Rank + dr, f = pos.File + df;
            while (r >= 0 && r < 8 && f >= 0 && f < 8) {
                if(b->pieceAtSquare(f, r)){
                    Colour targetColour = b->pieceAtSquare(f, r).getColour();
                    if (targetColour == c) return false; // Stop if own piece
                    if (f == p.File && r == p.Rank) return true; // return true if the target square is reached
                    return false; // Stop if opponent's piece
                }
                r += dr;
                f += df;
            }
            return false; //should only reach here if the target square is not reachable in the given direction
        }

};


