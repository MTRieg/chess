#include "boardobserver.h"
#include "piece.h"
#include <string>

MoveInfo::MoveInfo(Position oldPos, Piece *piece, 
                   Piece *capturedPiece, bool isEnPassant, bool isPromotion)
    : oldPos{oldPos}, piece{piece}, capturedPiece{capturedPiece},
        isEnPassant{isEnPassant}, isPromotion{isPromotion} {}

MoveInfo::MoveInfo() : oldPos{0,0}, piece{nullptr}, capturedPiece{nullptr},
                       isEnPassant{false}, isPromotion{false} {}

std::string MoveInfo::algebraic() const{
    return algebraicNotation;
}

//return colour of move (undefined behaviour when given invalid move)
const Colour MoveInfo::colour(){
    if(!piece){return Colour::White;}//anti-crash code, 
    return piece->getColour();
}


