#pragma once
#include <vector>
#include "piece.h"
#include "boardobserver.h"
using namespace std;


class Board {

    int size = 8;
    Piece*** board;
    bool setupMode = true; //will be set to false by either init() or leaveSetupMode()

    vector<BoardObserver*> observers;
    bool checkCache = false, checkmateCache = false, stalemateCache = false;
    InvisibleBoardInfo ibi;

    void notifyObserversMove(MoveInfo latest);
    void notifyObserversUndo(MoveInfo latest);
    void notifyObserversSetup(MoveInfo latest);

    void initHomeRow(Colour c, int row);

    //the four of these could be made boolean, but the public functions are 
    //required to use these private functions responsibly.
    
    //directly moves a piece to a new position, without any additional checks/effects
    void applyMove(const Position &oldPos, const Position &newPos, Piece *capturedPiece = nullptr);
    //still no notifications for observers or checks for validity, but does handle castling, en passant, and promotion
    void applyMove(const MoveInfo& move); 

    //undoes a move, restoring the previous state of the board
    //this will only affect the board, not the observers or ibi
    void undoMove(const Position &oldPos, const Position &newPos, const Piece *const capturedPiece = nullptr);
    //properly undoes a move, restoring the previous state of the board (still no board notifications)
    void undoMove(const MoveInfo& move);
    

    bool calculateCheck(Colour colour);
    bool calculateCheckmate(Colour colour, bool useCheckCache = true);
    bool calculateStalemate(Colour colour, bool useCheckCache = true);
    bool reevaluateCheckCache(Colour colour);
    bool reevaluateCheckmateCache(Colour colour, bool useCheckCache = true);
    bool reevaluateStalemateCache(Colour colour, bool useCheckCache = true, bool useCheckmateCache = true);
    void reevaluateCheckStalemateCheckmate(Colour colour);
    void updateCastlingRights();
    bool validNextMove(Colour colour) const;

    Board (const Board&);



    


    public:
    
    Board(int size = 8);
    ~Board();

    void init();
    int getSize() const;
    const vector<MoveInfo> getValidMoves(Colour colour) const;
    bool isValidMove(const MoveInfo& move, Board* tempBoard = nullptr) const;

    void addPiece(Piece* piece);
    void removePiece(Position pos);
    void leaveSetupMode(Colour colour);

    void movePiece(const MoveInfo& move);
    void undoAndNotify(const MoveInfo& move);
    

    void addObserver(BoardObserver* observer);
    void removeObserver(BoardObserver* observer);
    const Piece *pieceAtSquare(int file, int rank) const;
    const Piece *pieceAtPosition(const Position &pos) const;
    const bool check();
    const bool checkmate();
    const bool stalemate();
    const InvisibleBoardInfo BoardInfo() const;
    void updateAlgebraicNotation(const MoveInfo& move, const Board * const boardAfterMove = nullptr) const;
    MoveInfo moveInfo(Position oldPos, Position newPos, Piece::PieceType promotionType = Piece::PieceType::Queen) const;

};




