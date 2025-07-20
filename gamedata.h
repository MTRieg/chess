#include <vector>
#include "piece.h"
#include "boardobserver.h"
using namespace std;

class GameData : public BoardObserver{
    static const int DEFAULT_HISTORY_SIZE = 16;
    vector<MoveInfo> history;

    public:
    GameData() = default;
    void update(MoveInfo latest) override;
    vector<MoveInfo> latestMoves(int n = DEFAULT_HISTORY_SIZE);
    int gameLength() const;

};

