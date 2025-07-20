#include "textui.h"

TextUI::TextUI(Board* board, GameData* gameData)
    : AbstractUI(board, gameData) {

    // initialize the board display based on size
    boardDisplay.resize(board->getSize(), vector<char>(board->getSize(), '_'));

    // initialize the history display
    historyDisplay.resize(16, "");
}
