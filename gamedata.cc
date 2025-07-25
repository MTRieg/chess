#include <vector>
#include "gamedata.h"
#include <stdio.h>

// add latest move update from board
void GameData::moveUpdate(MoveInfo latest) {
    if(latest.piece == nullptr || latest.algebraic().empty()) {return;} //cannot be added to gameData
        history.push_back(latest);
    
}

void GameData::undoUpdate(MoveInfo latest) {
    if (history.empty()) { // No moves to undo
        cerr << "Undoing move that doesn't exist" << endl;
        return;
    }
    //code could be added here to make sure that the move being undoed matches the last move in history
    //but this shouldn't be necessary
    history.pop_back(); // Remove the last move
}

void GameData::setupUpdate(MoveInfo latest) {
    // Setup updates are not tracked in history, so we do nothing here
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

