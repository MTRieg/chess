#include <vector>
#include "gamedata.h"

// add latest move update from board
void GameData::update(MoveInfo latest) {
    if(latest.piece != nullptr && !latest.algebraic().empty()) { 
        //if it's a real move, not just a board update
        history.push_back(latest);
    }
    
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

// total steps in game so far
int GameData::gameLength() const {
    return history.size();
}

