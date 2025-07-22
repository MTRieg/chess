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

Board::~Board() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            delete board[i][j]; // Delete each piece
        }
        delete[] board[i]; // Delete each row
    }
    delete[] board; // Delete the board array
}


Board::Board(const Board& other) {
    if(this == &other) {
        return; // Handle self-assignment
    }

    // Clean up existing resources
    for (int i = 0; i < size; ++i) {
        delete[] board[i];
    }
    delete[] board;

    // Copy the other board's state
    size = other.size;
    board = new Piece**[size];
    for (int i = 0; i < size; ++i) {
        board[i] = new Piece*[size];
        for (int j = 0; j < size; ++j) {
            if (other.board[i][j]) {
                board[i][j] = other.board[i][j]->clone();
            } else {
                board[i][j] = nullptr;
            }
        }
    }
}



void Board::init() {
    for(int i=0; i<8; i++){
        board[i][1] = new Pawn(Colour::White, Position{i, 1}, this);
        board[i][6] = new Pawn(Colour::Black, Position{i, 6}, this);
    }
    initHomeRow(Colour::White, 0);
    initHomeRow(Colour::Black, 7);
}

void Board::initHomeRow(Colour c, int row) {
    board[0][row] = new Rook(c, Position{0, row}, this);
    board[1][row] = new Knight(c, Position{1, row}, this);
    board[2][row] = new Bishop(c, Position{2, row}, this);
    board[3][row] = new Queen(c, Position{3, row}, this);
    board[4][row] = new King(c, Position{4, row}, this);
    board[5][row] = new Bishop(c, Position{5, row}, this);
    board[6][row] = new Knight(c, Position{6, row}, this);
    board[7][row] = new Rook(c, Position{7, row}, this);
}

const Piece *Board::pieceAtSquare(int file, int rank) const {
    if (file < 0 || file >= size || rank < 0 || rank >= size) {
        return nullptr; // Out of bounds
    }
    return board[file][rank];
}

const Piece *Board::pieceAtPosition(const Position &pos) const {
    return pieceAtSquare(pos.File, pos.Rank);
}

const bool Board::check() {
    return checkCache;
}

const bool Board::checkmate() {
    return checkmateCache;
}

const InvisibleBoardInfo Board::BoardInfo() const{
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


void Board::movePiece(const MoveInfo& move) {
    if (!isValidMove(move)) {
        throw std::invalid_argument("Invalid move");
    }

    updateAlgebraicNotation(move);
    

    applyMove(move);
    notifyObservers(move);

    // Update check and checkmate status
    setCheckCache(calculateCheck(move.piece->getColour()), move.piece->getColour());
    setCheckmateCache(calculateCheckmate(move.piece->getColour()), move.piece->getColour());
}



const std::vector<MoveInfo> Board::getValidMoves(Colour colour) const {
    std::vector<MoveInfo> validMoves;
    Board *tempBoard = new Board(*this); // Create a temporary copy of the board
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->getColour() == colour) {
                std::vector<Position> moves = piece->validMoves();
                for (const Position& move : moves) {
                    auto capturedPiece = pieceAtPosition(move)->clone();
                    bool passant = false;
                    if(piece->getType() == Piece::PieceType::Pawn && ibi.enPassantFile == move.File && 
                      move.Rank == (colour == Colour::White ? 5 : 2)){
                        Position passanted_position = {move.File, move.Rank + (colour == Colour::White ? 1 : -1)};
                        capturedPiece = pieceAtPosition(passanted_position)->clone();
                        passant = true;
                    }
                    auto moveInfo = MoveInfo{piece->getPosition(), piece, capturedPiece, passant, };
                    if (tempBoard->isValidMove(moveInfo, tempBoard)) {
                        validMoves.push_back(moveInfo);
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









//resets tempBoard after use, so it can be reused for multiple calls
bool Board::isValidMove(const MoveInfo& move, Board* tempBoard) const{
    const Piece* piece = move.piece;
    if (!piece) return false;

    if((pieceAtPosition(move.oldPos)->getType() != move.piece->getType()) ||
                (pieceAtPosition(move.oldPos))->getColour() != move.capturedPiece->getColour()){
                    return false; //the original piece is not where they say it is now
            }

    // Check if the target square is occupied by a piece of the same colour
    const Piece* targetPiece = pieceAtPosition(move.piece->getPosition());
    if (targetPiece && targetPiece->getColour() == piece->getColour()) {
        return false;
    }

    // Check if the move is valid for the piece type
    if (!pieceAtPosition(piece->getPosition())->verifyMove(move.piece->getPosition())) {
        return false;
    }

    if(move.capturedPiece && (
            (pieceAtPosition(move.capturedPiece->getPosition())->getType() != move.capturedPiece->getType()) ||
                (pieceAtPosition(move.capturedPiece->getPosition()))->getColour() != move.capturedPiece->getColour())){
                    return false; //the captured piece is not where they say it is
            }

    bool inCheck;

    if(!tempBoard) {
        tempBoard = new Board(*this); // If no temporary board is provided, create a copy of the current board
        tempBoard->applyMove(move); // Temporarily apply the move to check for check or checkmate
        inCheck = tempBoard->calculateCheck(piece->getColour());
    }else{
        tempBoard->applyMove(move); // Temporarily apply the move to check for check or checkmate
        inCheck = tempBoard->calculateCheck(piece->getColour());
        tempBoard->undoMove(move);
    }

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
        Position promotionPos = newPos;
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


void Board::applyMove(const Position &oldPos, const Position &newPos, Piece *capturedPiece) {
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
void Board::undoMove(const Position &oldPos, const Position &newPos, const Piece *const capturedPiece) {
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
        board[capturedPiece->getPosition().File][capturedPiece->getPosition().Rank] = capturedPiece->clone(); // Restore captured piece
    }
}

void Board::undoMove(const MoveInfo& move) {
    Position oldPos = move.oldPos;
    Position newPos = move.piece->getPosition();
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
                undoMove(Position{7, 0}, Position{5, 0}); // Restore the rook
            } else if(newPos.File == 2) { // Queenside castle
                undoMove(Position{0, 0}, Position{3, 0}); // Restore the rook
            }
        } else {
            if(newPos.File == 6) { // Kingside castle
                undoMove(Position{7, 7}, Position{5, 7}); // Restore the rook
            } else if(newPos.File == 2) { // Queenside castle
                undoMove(Position{0, 7}, Position{3, 7}); // Restore the rook
            }
        }
    }

    ibi = move.ibi;

}

bool Board::calculateCheck(Colour colour){
    for(int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->getColour() != colour) {
                std::vector<Position> moves = piece->validMoves();
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


bool Board::calculateCheckmate(Colour colour, bool useCheckCache) {
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


void Board::reevaluateCheckAndCheckmate(Colour colour) {
    setCheckCache(calculateCheck(colour), colour);
    setCheckmateCache(calculateCheckmate(colour), colour);
}



//assumes move is a valid move, and check and checkmate variables in boardAfterMove have been updated)
void Board::updateAlgebraicNotation(const MoveInfo& move, const Board * const boardAfterMove) const {
    

    move.algebraicNotation = "";
    if (!move.piece) {
        return;
    }

    if (move.piece->getType() == Piece::PieceType::King && 
       (std::abs(move.oldPos.File - move.piece->getPosition().File) == 2)) {
         move.algebraicNotation += (move.piece->getPosition().File == 6) ? "O-O" : "O-O-O"; 
    }else if (move.isPromotion){
        if(move.capturedPiece) {
            move.algebraicNotation = Algebraic(move.oldPos)[0] + "x"; //add the file of the piece
        }
        move.algebraicNotation += Algebraic(move.piece->getPosition()) + " = " + firstChar(move.piece->getType());
    }else if(move.piece->getType() == Piece::PieceType::Pawn) {
        if(move.capturedPiece) {
            move.algebraicNotation = Algebraic(move.piece->getPosition())[0] + "x"; //add the file of the piece
        }
        move.algebraicNotation += Algebraic(move.oldPos);
    }else{
        move.algebraicNotation += firstChar(move.piece->getType());
        
        bool equivalentMoveSameFile = false;
        bool equivalentMoveSameRank = false;

        //pieces in the same file as move.piece
        for(int i=0; i<size && !equivalentMoveSameFile; i++){
            if(i != move.oldPos.Rank){ //if it's not move.piece
                //loop over all squares the piece at that square can move to
                for( Position positionPieceCanMove : pieceAtSquare(move.oldPos.File, i)->validMoves()){
                    if(positionPieceCanMove.Rank == move.piece->getPosition().Rank
                        &&positionPieceCanMove.File == move.piece->getPosition().File){
                            equivalentMoveSameFile = true;
                            break;
                    }
                }
            }
        }

        //pieces in the same rank as move.piece

        for(int i=0; i<size && !equivalentMoveSameRank; i++){
            if(i != move.oldPos.File){ //if it's not move.piece
                //loop over all squares the piece at that square can move to
                for( Position positionPieceCanMove : pieceAtSquare(i, move.oldPos.Rank)->validMoves()){
                    if(positionPieceCanMove.Rank == move.piece->getPosition().Rank
                        &&positionPieceCanMove.File == move.piece->getPosition().File){
                            equivalentMoveSameRank = true;
                            break;
                    }
                }
            }
        }

        if(equivalentMoveSameFile && equivalentMoveSameRank){
            move.algebraicNotation += Algebraic(move.oldPos);
        }else if(equivalentMoveSameFile){
            move.algebraicNotation += Algebraic(move.oldPos)[0];
        }else if(equivalentMoveSameRank){
            move.algebraicNotation += Algebraic(move.oldPos)[1];
        }




        if (move.capturedPiece) {
            move.algebraicNotation += "x";
        }
        move.algebraicNotation += Algebraic(move.oldPos);
    }


    if(boardAfterMove) {
        if (boardAfterMove->checkmateCache) {
            move.algebraicNotation += "#";
        }else if (boardAfterMove->checkCache){
            move.algebraicNotation += "+";
        }
    } else {
        Board *tempBoard = new Board(*this);
        tempBoard->applyMove(move); 
        tempBoard->reevaluateCheckAndCheckmate(move.piece->getColour());


        if (tempBoard->calculateCheck(move.piece->getColour())) {
            tempBoard->setCheckCache(true, move.piece->getColour());
            if (tempBoard->calculateCheckmate(move.piece->getColour())) {
                move.algebraicNotation += "#";
            } else {
                move.algebraicNotation += "+";
            }
        }
    }
    
    
}

MoveInfo &Board::moveInfo(Position oldPos, Position newPos, Piece::PieceType promotionType = Piece::PieceType::Queen){


    auto piece = pieceAtPosition(oldPos)->clone();
    auto capturedPiece = pieceAtPosition(newPos)->clone();
    bool passant = false;
    bool promotion = false;
    if(piece->getType() == Piece::PieceType::Pawn && ibi.enPassantFile == newPos.File && 
        newPos.Rank == (piece->getColour() == Colour::White ? 5 : 2)){
        Position passanted_position = {newPos.File, newPos.Rank + (piece->getColour() == Colour::White ? 1 : -1)};
        capturedPiece = pieceAtPosition(passanted_position)->clone();
        passant = true;
    }

    if(piece->getType() == Piece::PieceType::Pawn && (newPos.Rank == 0 || newPos.Rank == 7)){
        delete piece;
        switch(promotionType){
            case Piece::PieceType::Knight:
                piece = new Knight(pieceAtPosition(oldPos)->getColour(), newPos, this);
                break;
            case Piece::PieceType::Bishop:
                piece = new Bishop(pieceAtPosition(oldPos)->getColour(), newPos, this);
                break;
            case Piece::PieceType::Rook:
                piece = new Rook(pieceAtPosition(oldPos)->getColour(), newPos, this);
                break;
            default:
                piece = new Queen(pieceAtPosition(oldPos)->getColour(), newPos, this);
        }
        promotion = true;

    }


    auto moveInfo = MoveInfo{piece->getPosition(), piece, capturedPiece, passant, promotion};
    


}




