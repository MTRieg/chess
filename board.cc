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
    
    size = other.size;
    board = new Piece**[size];
    for (int i = 0; i < size; ++i) {
        board[i] = new Piece*[size];
        for (int j = 0; j < size; ++j) {
            if (other.board[i][j]) {
                board[i][j] = clonePiece(other.board[i][j], this);
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
    setupMode = false; // Set to false after initialization
    notifyObservers(MoveInfo());
    // do not need to use leaveSetupMode() here because the code creates a valid non-check board
}

void Board::addPiece(Piece* piece) {
    if (piece == nullptr) {
        throw std::invalid_argument("Cannot add a null piece to the board.");
    }
    Position pos = piece->getPosition();
    if (pos.File < 0 || pos.File >= size || pos.Rank < 0 || pos.Rank >= size) {
        throw std::out_of_range("Piece position is out of bounds.");
    }
    board[pos.File][pos.Rank] = piece;
    notifyObservers(MoveInfo{pos, piece, nullptr, false, false});
}

void Board::removePiece(Position pos) {
    if (pos.File < 0 || pos.File >= size || pos.Rank < 0 || pos.Rank >= size) {
        throw std::out_of_range("Piece position is out of bounds.");
    }
    if (board[pos.File][pos.Rank] == nullptr) {
        throw std::runtime_error("No piece at the specified position to remove.");
    }
    //piece is not deleted because piece is sent as the captured piece in moveInfo
    auto removedPiece = board[pos.File][pos.Rank];
    board[pos.File][pos.Rank] = nullptr;
    notifyObservers(MoveInfo{pos, nullptr, removedPiece, false, false});
}




void Board::leaveSetupMode(Colour colour) {
    //with more development time, you could add more checks to make sure the position is valid
    //for now, I will just calculate check or checkmate and making sure castling rights are correct
    if(!setupMode){
        cerr << "Warning: leaveSetupMode called when setupMode is already false." << endl;
        return;
    }
    updateCastlingRights();

    checkCache = calculateCheck(colour);
    checkmateCache = calculateCheckmate(colour);
    setupMode = false;
}

//only makes sure that, if the pieces are not on their home square, they cannot be involved in castling
void Board::updateCastlingRights(){
    if (pieceAtSquare(4, 0) == nullptr ||
        pieceAtSquare(4, 0)->getType() != Piece::PieceType::King ||
        pieceAtSquare(4, 0)->getColour() != Colour::White) {
        ibi.wkc = false;
        ibi.wqc = false;
    }
    if(pieceAtSquare(4, 7) == nullptr ||
        pieceAtSquare(4, 7)->getType() != Piece::PieceType::King ||
        pieceAtSquare(4, 7)->getColour() != Colour::Black) {
        ibi.bkc = false;
        ibi.bqc = false;
    }
    if(pieceAtSquare(0,0) == nullptr ||
        pieceAtSquare(0,0)->getType() != Piece::PieceType::Rook ||
        pieceAtSquare(0,0)->getColour() != Colour::White) {
        ibi.wqc = false;
    }
    if(pieceAtSquare(7,0) == nullptr ||
        pieceAtSquare(7,0)->getType() != Piece::PieceType::Rook ||
        pieceAtSquare(7,0)->getColour() != Colour::Black) {
        ibi.wkc = false;
    }
    if(pieceAtSquare(0,7) == nullptr ||
        pieceAtSquare(0,7)->getType() != Piece::PieceType::Rook ||
        pieceAtSquare(0,7)->getColour() != Colour::White) {
        ibi.bqc = false;
    }
    if(pieceAtSquare(7,7) == nullptr ||
        pieceAtSquare(7,7)->getType() != Piece::PieceType::Rook ||
        pieceAtSquare(7,7)->getColour() != Colour::Black) {
        ibi.bkc = false;
    }
    return;
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

const bool Board::stalemate() {
    return stalemateCache;
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


void updateBoardInfo(InvisibleBoardInfo &ibi, const MoveInfo &move) {
    Colour colour = move.piece->getColour();
    if(move.piece->getType() == Piece::PieceType::King) {
        if(colour == Colour::White) {
            ibi.wkc = false;
            ibi.wqc = false;
        } else {
            ibi.bkc = false;
            ibi.bqc = false;
        }
    }else if(move.piece->getType() == Piece::PieceType::Rook) {
        int rookRank = ((colour == Colour::White) ? 0 : 7);
        if(move.oldPos.Rank == rookRank) {
            if(move.oldPos.File == 0) {
                if(colour == Colour::White) {
                    ibi.wqc = false;
                } else {
                    ibi.bqc = false;
                }
            } else if(move.oldPos.File == 7) {
                if(colour == Colour::White) {
                    ibi.wkc = false;
                } else {
                    ibi.bkc = false;
                }
            }
        }
    }
    else if(abs(move.oldPos.Rank - move.piece->getPosition().Rank) == 2 && 
            move.piece->getType() == Piece::PieceType::Pawn) {
        ibi.enPassantFile = move.piece->getPosition().File;
    }
}



void Board::movePiece(const MoveInfo& move) {
    if (!isValidMove(move)) {
        throw std::invalid_argument("Invalid move");
    }
    
    updateAlgebraicNotation(move);
    

    applyMove(move);

    updateBoardInfo(ibi, move);

    notifyObservers(move);


    Colour oppColour = (move.piece->getColour() == Colour::White) ? Colour::Black : Colour::White;
    // Update check and checkmate status
    reevaluateCheckStalemateCheckmate(oppColour);
}





const std::vector<MoveInfo> Board::getValidMoves(Colour colour) const {
    std::vector<MoveInfo> validMoves;
    //Board *tempBoard = new Board(*this); // Create a temporary copy of the board
    //tried using this as part of isValidMove, but it seemed to not be working correctly
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->getColour() == colour) {
                std::vector<Position> moves = piece->validMoves();
                for (const Position& destination : moves) {
                    auto move = moveInfo({i,j},destination);
                    if(isValidMove(move)){
                        validMoves.push_back(move);
                        if(move.isPromotion){
                            validMoves.push_back(moveInfo({i,j},destination, Piece::PieceType::Bishop));
                            validMoves.push_back(moveInfo({i,j},destination, Piece::PieceType::Knight));
                            validMoves.push_back(moveInfo({i,j},destination, Piece::PieceType::Rook));
                        }
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

    if(!piece->getPosition().inBounds(size) || !move.oldPos.inBounds(size)) {
        return false; // Invalid position
    }

    if(!comparePieces(pieceAtPosition(move.oldPos), move.piece)){
                    return false; //the original piece is not where they say it is now
            }

    // Check if the target square is occupied by a piece of the same colour
    const Piece* targetPiece = pieceAtPosition(move.piece->getPosition());
    if (targetPiece && targetPiece->getColour() == piece->getColour()) {
        return false;
    }

    // Check if the move is valid for the piece type
    if (!pieceAtPosition(move.oldPos)->verifyMove(move.piece->getPosition())) {
        return false;
    }

    if(move.capturedPiece && comparePieces(pieceAtPosition(move.capturedPiece->getPosition()), move.capturedPiece)){
                    return false; //the captured piece is not where they say it is
            }

    bool inCheck = false;
    
    

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
                applyMove(Position{7, 0}, Position{5, 0}); 
            } else if(newPos.File == 2) { // Queenside castle
                applyMove(Position{0, 0}, Position{3, 0});
            }
        } else {
            if(newPos.File == 6) { // Kingside castle
                applyMove(Position{7, 7}, Position{5, 7});
            } else if(newPos.File == 2) { // Queenside castle
                applyMove(Position{0, 7}, Position{3, 7});
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
    if(capturedPiece) {
        delete pieceAtPosition(capturedPiece->getPosition()); // Delete captured piece if any
    }
    board[newPos.File][newPos.Rank] = movedPiece; // Place at new position
    board[oldPos.File][oldPos.Rank] = nullptr; // Remove from old position

    
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
        board[capturedPiece->getPosition().File][capturedPiece->getPosition().Rank] = clonePiece(capturedPiece); // Restore captured piece
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

//colour is colour that might be in check
bool Board::calculateCheck(Colour colour){
    Position kingPosition = {0,0};
    for(int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            auto piece = pieceAtSquare(i,j);
            if(piece && piece->getType() == Piece::PieceType::King && piece->getColour() == colour){
                kingPosition = {i,j};
            }
        }
    }

    for(int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->getColour() != colour && piece->verifyMove(kingPosition)) {
                    return true;
            }
        }
    }
    return false; // No check found

}

bool Board::calculateStalemate(Colour colour, bool useCheckCache) {
    if ((useCheckCache && checkCache) || (!useCheckCache && calculateCheck(colour))) {
        return false; // Cannot be stalemate if in check
    }

    return !validNextMove(colour); // return if there are no valid next moves
}

bool Board::reevaluateCheckCache(Colour colour) {
    checkCache = calculateCheck(colour);
    return checkCache;
}

bool Board::reevaluateCheckmateCache(Colour colour, bool useCheckCache) {
    checkmateCache = calculateCheckmate(colour, useCheckCache);
    return checkmateCache;
}

bool Board::reevaluateStalemateCache(Colour colour, bool useCheckCache, bool useCheckmateCache) {
    if (useCheckmateCache && checkmateCache){
        stalemateCache = false; // If checkmate is true, stalemate cannot be true
        return stalemateCache;
    }
    stalemateCache = calculateStalemate(colour, useCheckCache);
    return stalemateCache;
}


bool Board::calculateCheckmate(Colour colour, bool useCheckCache) {
    if ((useCheckCache && !checkCache) || (!useCheckCache && !calculateCheck(colour))) {
        return false; // Cannot be checkmate if not in check
    }
    
    return !validNextMove(colour);
}

bool Board::validNextMove(Colour colour) const {
    // Check if the current player has any valid moves
    std::vector<MoveInfo> validMoves = getValidMoves(colour);
    return !validMoves.empty();
}



void Board::reevaluateCheckStalemateCheckmate(Colour colour) {
    if(calculateCheck(colour)){
        if(validNextMove(colour)) {
            checkCache = true;
            checkmateCache = false;
            stalemateCache = false;
        } else {
            checkCache = true;
            checkmateCache = true;
            stalemateCache = false;
        }
    }else{
        checkCache = false;
        if(validNextMove(colour)) {
            checkmateCache = false;
            stalemateCache = false;
        } else {
            checkmateCache = false;
            stalemateCache = true;
        }
    }
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
        move.algebraicNotation += Algebraic(move.piece->getPosition());
    }else{
        move.algebraicNotation += firstChar(move.piece->getType());
        
        bool equivalentMoveSameFile = false;
        bool equivalentMoveSameRank = false;

        //pieces in the same file as move.piece
        for(int i=0; i<size && !equivalentMoveSameFile; i++){
            if(i != move.oldPos.Rank //if it's not move.piece
                && pieceAtSquare(move.oldPos.File, i)
                && pieceAtSquare(move.oldPos.File, i)->getColour() == move.piece->getColour()){
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
            if(i != move.oldPos.File //if it's not move.piece
                && pieceAtSquare(i, move.oldPos.Rank)
                && pieceAtSquare(i, move.oldPos.Rank)->getColour() == move.piece->getColour()){
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
        move.algebraicNotation += Algebraic(move.piece->getPosition());
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
        tempBoard->reevaluateCheckStalemateCheckmate(move.piece->getColour());


        if (tempBoard->reevaluateCheckCache(move.piece->getColour())) {
            if (tempBoard->reevaluateCheckmateCache(move.piece->getColour(), true)) {
                move.algebraicNotation += "#";
            } else {
                move.algebraicNotation += "+";
            }
        }
    }
    
    
}

MoveInfo Board::moveInfo(Position oldPos, Position newPos, Piece::PieceType promotionType) const{


    auto piece = clonePiece(pieceAtPosition(oldPos));
    piece->move(newPos);
    auto capturedPiece = clonePiece(pieceAtPosition(newPos));
    bool passant = false;
    bool promotion = false;
    if(piece->getType() == Piece::PieceType::Pawn && ibi.enPassantFile == newPos.File && 
        newPos.Rank == (piece->getColour() == Colour::White ? 5 : 2)){
        Position passanted_position = {newPos.File, newPos.Rank + (piece->getColour() == Colour::White ? 1 : -1)};
        capturedPiece = clonePiece(pieceAtPosition(passanted_position));
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


    auto moveInfo = MoveInfo{oldPos, piece, capturedPiece, passant, promotion};
    
    return moveInfo;


}




