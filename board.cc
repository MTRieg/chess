#include "board.h"
#include "boardobserver.h"
#include "gamedata.h"
#include "piece.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include <algorithm>
#include <vector>


Board::Board(int size_in = 8) : size{size_in}{
    board = new Piece**[size];
    for (int i = 0; i < size; ++i) {
        board[i] = new Piece*[size];
        for (int j = 0; j < size; ++j) {
            board[i][j] = nullptr;
        }
    }
}


void Board::init() {
    for(int i=0; i<8; i++){
        board[i][1] = new Pawn(Colour::White, Piece::Position{i, 1}, this);
        board[i][6] = new Pawn(Colour::Black, Piece::Position{i, 6}, this);
    }
    InitHomeRow(Colour::White, 0);
    InitHomeRow(Colour::Black, 7);
}



void Board::InitHomeRow(Colour c, int row) {
    board[0][row] = new Rook(c, Piece::Position{0, row}, this);
    board[1][row] = new Knight(c, Piece::Position{1, row}, this);
    board[2][row] = new Bishop(c, Piece::Position{2, row}, this);
    board[3][row] = new Queen(c, Piece::Position{3, row}, this);
    board[4][row] = new King(c, Piece::Position{4, row}, this);
    board[5][row] = new Bishop(c, Piece::Position{5, row}, this);
    board[6][row] = new Knight(c, Piece::Position{6, row}, this);
    board[7][row] = new Rook(c, Piece::Position{7, row}, this);
}

const Piece *Board::pieceAtSquare(int file, int rank) const {
    if (file < 0 || file >= size || rank < 0 || rank >= size) {
        return nullptr; // Out of bounds
    }
    return board[file][rank];
}

const Piece *Board::pieceAtPosition(const Piece::Position &pos) const {
    return pieceAtSquare(pos.File, pos.Rank);
}

const bool Board::check() {
    return check;
}

const bool Board::checkmate() {
    return checkmate;
}

const InvisibleBoardInfo Board::BoardInfo() {
    return ibi;
}

void Board::notifyObservers(MoveInfo latest) {
    for (BoardObserver* observer : observers) {
        observer->update(latest);
    }
}

void Board::addObserver(BoardObserver* observer) {
    observers.push_back(observer);
}

void Board::removeObserver(BoardObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

const std::vector<MoveInfo> Board::getValidMoves(Colour colour) const {
    std::vector<MoveInfo> validMoves;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->getColour() == colour) {
                std::vector<Piece::Position> moves = piece->validMoves();
                for (const auto& move : moves) {


                    if (isValidMove(MoveInfo(piece->getPosition(), piece, pieceAtPosition(move)))) {
                        validMoves.push_back(MoveInfo(piece->getPosition(), piece, pieceAtPosition(move)));
                    }
                }
            }
        }
    }
    return validMoves;
}

bool Board::isValidMove(const MoveInfo& move) const {
    const Piece* piece = move.piece;
    if (!piece) return false;

    // Check if the target square is occupied by a piece of the same colour
    const Piece* targetPiece = pieceAtPosition(move.target);
    if (targetPiece && targetPiece->getColour() == piece->getColour()) {
        return false;
    }

    // Check if the move is valid for the piece type
    if (!piece->isValidMove(move.target)) {
        return false;
    }


    

}
    

    





