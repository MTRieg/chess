#include <vector>
#include "piece.h"
#include "gamedata.h"
#include "boardobserver.h"

class Board {

    int SIZE = 8;
    Piece*** board;

    BoardObserver** observers;
    GameData *gameData;

    void notifyObservers();

    public:

    Board(int size = 8);
    ~Board();

    void init();
    vector<MoveInfo> getValidMoves(Piece::Colour colour) const;
    bool isValidMove(const MoveInfo& move) const;
    void movePiece(const MoveInfo& move);
    void addObserver(BoardObserver* observer);
    void removeObserver(BoardObserver* observer);

    private:



};

struct InvisibleBoardInfo {
    bool wkc, wqc, bkc, bqc; // castling ability
};
