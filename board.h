#include <vector>
#include "piece.h"
#include "boardobserver.h"

#ifndef INVISIBLE_BOARD_INFO
#define INVISIBLE_BOARD_INFO
struct InvisibleBoardInfo {
    bool wkc = true, wqc = true, bkc = true, bqc = true; // castling ability
    int enPassantFile = -1;
};
#endif


class Board {

    int size = 8;
    Piece*** board;

    vector<BoardObserver*> observers;
    bool check = false, checkmate = false;
    InvisibleBoardInfo ibi;

    void notifyObservers(MoveInfo latest);
    void InitHomeRow(Colour c, int row);

    //directly applies the move to the pieces, without checking validity or notifying observers
    void applyMove(const MoveInfo& move); 
    void undoMove(const MoveInfo& move);



    


    public:
    
    Board(int size = 8);
    ~Board();

    void init();
    int getSize() const;
    const vector<MoveInfo> getValidMoves(Colour colour) const;
    const bool isValidMove(const MoveInfo& move) const;
    void movePiece(const MoveInfo& move);
    void addObserver(BoardObserver* observer);
    void removeObserver(BoardObserver* observer);
    const Piece *pieceAtSquare(int file, int rank) const;
    const Piece *pieceAtPosition(const Piece::Position &pos) const;
    const bool check();
    const bool checkmate();
    const InvisibleBoardInfo BoardInfo();


};


