#pragma once
#include <string>
#include "piece.h"
using namespace std;

#ifndef INVISIBLE_BOARD_INFO
#define INVISIBLE_BOARD_INFO
struct InvisibleBoardInfo {
    bool wkc = true, wqc = true, bkc = true, bqc = true; // castling ability
    int enPassantFile = -1;
};
#endif

struct MoveInfo {
    Piece::Position oldPos;
    Piece *piece; // the piece after it has been moved
    Piece *capturedPiece;
    bool isEnPassant = false;
    bool isPromotion = false;
    InvisibleBoardInfo ibi; //castling rights and en passant info from before the move was made

    MoveInfo(Piece::Position oldPos, Piece *piece, 
             Piece *capturedPiece = nullptr,
             bool isEnPassant = false, bool isPromotion = false);

    string algebraic() const;

};

class BoardObserver {

    public:
    BoardObserver() = default;
    virtual ~BoardObserver() = 0;
    virtual void update(MoveInfo latest) = 0;

};
