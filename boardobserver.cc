#include "boardobserver.h"
#include "piece.h"
#include <string>

MoveInfo::MoveInfo(Position oldPos, Piece *piece, 
                   Piece *capturedPiece, bool isEnPassant, bool isPromotion)
    : oldPos{oldPos}, piece{piece}, capturedPiece{capturedPiece},
        isEnPassant{isEnPassant}, isPromotion{isPromotion} {}

MoveInfo::MoveInfo() : oldPos{0,0}, piece{nullptr}, capturedPiece{nullptr},
                       isEnPassant{false}, isPromotion{false} {}

MoveInfo::~MoveInfo() {
    delete piece; 
    delete capturedPiece; 
}

MoveInfo::MoveInfo(const MoveInfo& other)
    : oldPos{other.oldPos}, piece{other.piece ? other.piece->clone() : nullptr},
      capturedPiece{other.capturedPiece ? other.capturedPiece->clone() : nullptr},
      isEnPassant{other.isEnPassant}, isPromotion{other.isPromotion}, ibi{other.ibi} {}

MoveInfo& MoveInfo::operator=(const MoveInfo& other) {
    if (this != &other) {
        oldPos = other.oldPos;
        delete piece;
        piece = other.piece ? other.piece->clone() : nullptr;
        delete capturedPiece;
        capturedPiece = other.capturedPiece ? other.capturedPiece->clone() : nullptr;
        isEnPassant = other.isEnPassant;
        isPromotion = other.isPromotion;
        ibi = other.ibi;
    }
    return *this;
}

MoveInfo::MoveInfo(MoveInfo&& other) noexcept
    : oldPos{other.oldPos}, piece{other.piece}, capturedPiece{other.capturedPiece},
      isEnPassant{other.isEnPassant}, isPromotion{other.isPromotion}, ibi{other.ibi} {
    other.piece = nullptr;
    other.capturedPiece = nullptr;
}

MoveInfo& MoveInfo::operator=(MoveInfo&& other) noexcept {
    if (this != &other) {
        oldPos = other.oldPos;
        delete piece;
        piece = other.piece;
        other.piece = nullptr;
        delete capturedPiece;
        capturedPiece = other.capturedPiece;
        other.capturedPiece = nullptr;
        isEnPassant = other.isEnPassant;
        isPromotion = other.isPromotion;
        ibi = other.ibi;
    }
    return *this;
}


std::string MoveInfo::algebraic() const{
    return algebraicNotation;
}

//return colour of move (undefined behaviour when given invalid move)
const Colour MoveInfo::colour(){
    if(!piece){return Colour::White;}//anti-crash code, 
    return piece->getColour();
}


