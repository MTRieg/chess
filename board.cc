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


Board::Board(int size_in) : size{size_in}{
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
    return checkCache;
}

const bool Board::checkmate() {
    return checkmateCache;
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

const bool Board::isValidMove(const MoveInfo& move) const {
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

    applyMove(move); // Temporarily apply the move to check for check or checkmate
    bool inCheck = calculateCheck(piece->getColour());

    undoMove(move); // Undo the temporary move

    if (inCheck) {
        return false; // Move puts own king in check, not valid
    }

    return true; // If all checks pass, the move is valid
}



void Board::applyMove(const MoveInfo& move) {

    auto newPos = move.piece->getPosition();
    auto oldPos = move.oldPos;

    applyMove(oldPos, newPos, move.capturedPiece);


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


void Board::applyMove(const Piece::Position &oldPos, const Piece::Position &newPos, Piece *capturedPiece = nullptr) {
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


//assumes capturedPiece is a valid pointer, and that the move is valid
void Board::undoMove(const Piece::Position &oldPos, const Piece::Position &newPos, const Piece *const capturedPiece = nullptr) {
    Piece *movedPiece = board[newPos.File][newPos.Rank];
    if (!movedPiece) {
        return; // No piece to undo
    }

    movedPiece->move(oldPos); // Update the piece's local position variables
    board[oldPos.File][oldPos.Rank] = movedPiece; // Place back at old position
    board[newPos.File][newPos.Rank] = nullptr; // Remove from new position 

    if (capturedPiece) {
        if(board[capturedPiece->getPosition().File][capturedPiece->getPosition().Rank] != nullptr) {
            delete board[capturedPiece->getPosition().File][capturedPiece->getPosition().Rank]; 
            //delete the piece at the captured position if it exists (should be redundant)
        }
        board[capturedPiece->getPosition().File][capturedPiece->getPosition().Rank] = capturedPiece; // Restore captured piece
    }
}

void Board::undoMove(const MoveInfo& move) {
    Piece::Position oldPos = move.oldPos;
    Piece::Position newPos = move.piece->getPosition();
    Piece *capturedPiece = move.capturedPiece;

    undoMove(oldPos, newPos, capturedPiece);
    // If the move was a promotion, we need to restore the original piece type
    if(move.isPromotion) {
        // Handle promotion undo logic
        auto colour = move.piece->getColour();
        delete pieceAtPosition(oldPos);
        board[oldPos.File][oldPos.Rank] = new Pawn(colour, oldPos, this); // Restore the original piece type
    }

    if(move.piece->getType() == Piece::PieceType::King && 
       (std::abs(oldPos.File - newPos.File) == 2)) { // Castling undo
        // Handle castling undo logic
        if(move.piece->getColour() == Colour::White) {
            if(newPos.File == 6) { // Kingside castle
                undoMove(Piece::Position{7, 0}, Piece::Position{5, 0}); // Restore the rook
            } else if(newPos.File == 2) { // Queenside castle
                undoMove(Piece::Position{0, 0}, Piece::Position{3, 0}); // Restore the rook
            }
        } else {
            if(newPos.File == 6) { // Kingside castle
                undoMove(Piece::Position{7, 7}, Piece::Position{5, 7}); // Restore the rook
            } else if(newPos.File == 2) { // Queenside castle
                undoMove(Piece::Position{0, 7}, Piece::Position{3, 7}); // Restore the rook
            }
        }
    }

    ibi = move.ibi;

}

const bool Board::calculateCheck(Colour colour){
    for(int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->getColour() != colour) {
                std::vector<Piece::Position> moves = piece->validMoves();
                for (const auto& move : moves) {
                    const Piece* targetPiece = pieceAtPosition(move);
                    if (targetPiece && targetPiece->getType() == Piece::PieceType::King && targetPiece->getColour() == colour) {
                        return true; // Check found
                    }
                }
            }
        }
    }
    return false; // No check found

}


const bool Board::calculateCheckmate(Colour colour, bool useCheckCache = true) {
    if ((useCheckCache && !checkCache) || !useCheckCache && !calculateCheck(colour)) {
        return false; // Cannot be checkmate if not in check
    }

    std::vector<MoveInfo> validMoves = getValidMoves(colour);
    if (validMoves.empty()) {
        return true;
    }
    
    return false;
}



void Board::setCheckCache(bool value, Colour colour) {
    if (colour == Colour::White) {
        checkCache = calculateCheck(Colour::White);
    } else {
        checkCache = calculateCheck(Colour::Black);
    }
}



void Board::setCheckmateCache(bool value, Colour colour) {
    if (colour == Colour::White) {
        checkmateCache = calculateCheckmate(Colour::White);
    } else {
        checkmateCache = calculateCheckmate(Colour::Black);
    }
}

