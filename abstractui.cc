#include "abstractui.h"

AbstractUI::AbstractUI(Board* board, GameData* gameData) : board{board}, gameData{gameData} {
    board->addObserver(this);
}

