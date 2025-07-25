#pragma once
#include <vector>
#include "piece.h"
#include "boardobserver.h"
using namespace std;

class GameData : public BoardObserver{
    static const int DEFAULT_HISTORY_SIZE = 16;
    vector<MoveInfo> history;

    public:
    GameData() = default;
    void clearHistory();
    void moveUpdate(MoveInfo latest) override;
    void undoUpdate(MoveInfo latest) override;
    void setupUpdate(MoveInfo latest) override;
    vector<MoveInfo> latestMoves(int n = DEFAULT_HISTORY_SIZE);
    int gameLength() const;

};



