#include <vector>
#include "piece.h"
#include "boardobserver.h"

#ifndef INVISIBLE_BOARD_INFO
#define INVISIBLE_BOARD_INFO
struct InvisibleBoardInfo {
    bool wkc, wqc, bkc, bqc; // castling ability
    int enPassantFile;
};
#endif


class Board {

    int SIZE = 8;
    Piece*** board;

    BoardObserver** observers;
    bool check;
    bool checkmate;
    InvisibleBoardInfo ibi;

    void notifyObservers();

    public:

    Board(int size = 8);
    ~Board();

    void init();
    const vector<MoveInfo> getValidMoves(Piece::Colour colour) const;
    const bool isValidMove(const MoveInfo& move) const;
    void movePiece(const MoveInfo& move);
    void addObserver(BoardObserver* observer);
    void removeObserver(BoardObserver* observer);
    const Piece &pieceAtSquare(int file, int rank);
    const bool check();
    const bool checkmate();
    const InvisibleBoardInfo BoardInfo();



};


