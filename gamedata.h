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

