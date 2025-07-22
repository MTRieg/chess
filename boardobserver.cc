#include "boardobserver.h"
#include "piece.h"
#include <string>

MoveInfo::MoveInfo(Position oldPos, Piece *piece, 
                   Piece *capturedPiece, bool isEnPassant, bool isPromotion)
    : oldPos{oldPos}, piece{piece}, capturedPiece{capturedPiece},
        isEnPassant{isEnPassant}, isPromotion{isPromotion} {}

std::string MoveInfo::algebraic() const{
    return algebraicNotation;
}


