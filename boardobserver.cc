#include "boardobserver.h"
#include "piece.h"

MoveInfo::MoveInfo(Piece::Position oldPos, Piece *piece, 
                   Piece *capturedPiece, bool isEnPassant, bool isPromotion)
    : oldPos{oldPos}, piece{piece}, capturedPiece{capturedPiece},
        isEnPassant{isEnPassant}, isPromotion{isPromotion} {}
