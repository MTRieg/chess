#include <string>
#include "piece.h"

struct MoveInfo {
    Piece::Position oldPos;
    Piece *piece;
    Piece *capturedPiece;
    bool isEnPassant = false;
    bool isPromotion = false;

    MoveInfo(Piece::Position oldPos, Piece *piece, 
             Piece *capturedPiece = nullptr,
             bool isEnPassant = false, bool isPromotion = false);

    std::string algebraic() const;

};

class BoardObserver {

    public:
    BoardObserver() = default;
    virtual ~BoardObserver() = 0;
    virtual void update(MoveInfo latest) = 0;

};
