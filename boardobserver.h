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
    Position oldPos;
    Piece *piece; // the piece after it has been moved
    //if piece is nullptr, then this tells the boardobservers to treat this as a flag to update their boards
    //though some observers may just always hard refresh from board
    Piece *capturedPiece;
    bool isEnPassant = false;
    bool isPromotion = false;
    InvisibleBoardInfo ibi; //castling rights and en passant info from before the move was made
    

    MoveInfo(Position oldPos, Piece *piece, 
             Piece *const capturedPiece = nullptr,
             bool isEnPassant = false, bool isPromotion = false);
    
    MoveInfo();

    const Colour colour();
    string algebraic() const;

    mutable string algebraicNotation;

};

class BoardObserver {

    public:
    BoardObserver() = default;
    virtual ~BoardObserver() = default;
    virtual void update(MoveInfo latest) = 0;

};
