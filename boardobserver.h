#pragma once
#include <string>
#include "piece.h"
using namespace std;

struct MoveInfo {
    const Piece::Position oldPos;
    const Piece *piece; // the piece after it has been moved
    const Piece *capturedPiece;
    bool isEnPassant = false;
    bool isPromotion = false;

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
