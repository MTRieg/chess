#include <vector>
#include "piece.h"
#include "boardobserver.h"

class GameData : public BoardObserver{
    vector<MoveInfo> history;

    public:
    GameData() = default;
    void update(MoveInfo latest) override;
    vector<MoveInfo> lastestMoves(int n = 16);

};

struct MoveInfo {
    Piece::Position oldPos;
    Piece::Piece *piece;
    Piece::CapturedPiece capturedPiece;
    bool isEnPassant = false;
    bool isPromotion = false;

    MoveInfo(Piece::Position oldPos, Piece::Piece *piece, 
             Piece::CapturedPiece capturedPiece = Piece::CapturedPiece::None,
             bool isEnPassant = false, bool isPromotion = false)
        : oldPos{oldPos}, piece{piece}, capturedPiece{capturedPiece},
          isEnPassant{isEnPassant}, isPromotion(isPromotion) {}
};


