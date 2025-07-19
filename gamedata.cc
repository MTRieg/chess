#include "gamedata.h"

// add latest move update from board
void GameData::update(MoveInfo lastest) {
    history.push_back(lastest);
}

// returns the last n moves from the game
vector<MoveInfo> GameData::latestMoves(int n) {
    if (n < 0) n = 0;
    if (n > history.size()) n = history.size();

    vector<MoveInfo> latestMoves;
    for (int i = history.size() - n; i < history.size(); ++i) {
        latestMoves.push_back(history[i]);
    }
    return latestMoves;
}
