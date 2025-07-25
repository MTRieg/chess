#pragma once
#include <vector>
#include <string>
#include "abstractui.h"
#include "board.h"
#include "gamedata.h"
using namespace std;

class TextUI : public AbstractUI {

    vector<vector<char>> boardDisplay;
    vector<string> historyDisplay;
    const static int historyWidth = 2; // default width of history ui

    const static int historyDepth = 8;

    void updateBoardDisplay(MoveInfo latest);
    void updateHistoryDisplay();

    public:
    TextUI(Board* board, GameData* gameData);

    void moveUpdate(MoveInfo latest);
    void undoUpdate(MoveInfo latest);
    void setupUpdate(MoveInfo latest);

    private:
    void output();

};
