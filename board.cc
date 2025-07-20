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

int Board::getSize() const {
    return size;
}

bool Board::isValidMove(const MoveInfo& move) const {
    const Piece* piece = move.piece;
    if (!piece) return false;

    // Check if the target square is occupied by a piece of the same colour
    const Piece* targetPiece = pieceAtPosition(move.piece->getPosition());
    if (targetPiece && targetPiece->getColour() == piece->getColour()) {
        return false;
    }

    // Check if the move is valid for the piece type
    if (!pieceAtPosition(piece->getPosition())->verifyMove(move.piece->getPosition())) {
        return false;
    }

    // Check if the move puts the king in check
    




    
    return true; // If all checks pass, the move is valid
}



void Board::applyMove(const MoveInfo& move) {

    auto newPos = move.piece->getPosition();
    auto oldPos = move.oldPos;

    movePieceDirectly(oldPos, newPos, move.capturedPiece);


    if(move.isPromotion){
        // Handle promotion logic here, e.g., change to a Queen
        Piece::Position promotionPos = newPos;
        delete pieceAtPosition(promotionPos);
        switch(move.piece->getType()) {
            case Piece::PieceType::Queen:
                board[promotionPos.File][promotionPos.Rank] = new Queen(move.piece->getColour(), promotionPos, this);
                break;
            case Piece::PieceType::Rook:
                board[promotionPos.File][promotionPos.Rank] = new Rook(move.piece->getColour(), promotionPos, this);
                break;
            case Piece::PieceType::Bishop:
                board[promotionPos.File][promotionPos.Rank] = new Bishop(move.piece->getColour(), promotionPos, this);
                break;
            case Piece::PieceType::Knight:
                board[promotionPos.File][promotionPos.Rank] = new Knight(move.piece->getColour(), promotionPos, this);
                break;
            default:
                // Default to Queen if no valid promotion type is specified
                board[promotionPos.File][promotionPos.Rank] = new Queen(move.piece->getColour(), promotionPos, this);
                break;
        }
       
    }


    if(move.piece->getType() == Piece::PieceType::King && 
       (std::abs(oldPos.File - newPos.File) == 2)) { // Castling move
        // Handle castling logic
        if(move.piece->getColour() == Colour::White) {
            if(newPos.File == 6) { // Kingside castle
                board[5][0] = board[7][0]; // Move the rook
                board[7][0] = nullptr; // Remove the rook from its old position
            } else if(newPos.File == 2) { // Queenside castle
                board[3][0] = board[0][0]; // Move the rook
                board[0][0] = nullptr; // Remove the rook from its old position
            }
        } else {
            if(newPos.File == 6) { // Kingside castle
                board[5][7] = board[7][7]; // Move the rook
                board[7][7] = nullptr; // Remove the rook from its old position
            } else if(newPos.File == 2) { // Queenside castle
                board[3][7] = board[0][7]; // Move the rook
                board[0][7] = nullptr; // Remove the rook from its old position
            }
        }
    }



    if(move.piece->getType() == Piece::PieceType::King) {
        if(move.piece->getColour() == Colour::White) {
            ibi.wkc = false; // White cannot kingside castle anymore
            ibi.wqc = false; // White cannot queenside castle anymore
        } else {
            ibi.bkc = false; // Black cannot kingside castle anymore
            ibi.bqc = false; // Black cannot queenside castle anymore
        }
    } else if(move.piece->getType() == Piece::PieceType::Rook) {
        if(move.piece->getColour() == Colour::White && oldPos.File == 0) {
            ibi.wqc = false; // White queen rook moved
        } else if(move.piece->getColour() == Colour::White && oldPos.File == 7) {
            ibi.wkc = false; // White king rook moved
        } else if(move.piece->getColour() == Colour::Black && oldPos.File == 0) {
            ibi.bqc = false; // Black queen rook moved
        } else if(move.piece->getColour() == Colour::Black && oldPos.File == 7) {
            ibi.bkc = false; // Black king rook moved
        }
    }

}


void Board::movePieceDirectly(const Piece::Position &oldPos, const Piece::Position &newPos, Piece *capturedPiece = nullptr) {
    Piece *movedPiece = board[oldPos.File][oldPos.Rank];
    if (!movedPiece) {
        return; // No piece to move
    }

    movedPiece->move(newPos); // Update the piece's local position variables
    delete pieceAtPosition(newPos); // Delete captured piece if any
    board[newPos.File][newPos.Rank] = movedPiece; // Place at new position
    board[oldPos.File][oldPos.Rank] = nullptr; // Remove from old position

    if (capturedPiece) {
        delete capturedPiece; // Delete captured piece
    }
}

    





