#include "textui.h"
#include "piece.h"
#include <math.h>

TextUI::TextUI(Board* board, GameData* gameData)
    : AbstractUI(board, gameData) {

    // initialize the board display based on size
    boardDisplay.resize(board->getSize(), vector<char>(board->getSize(), '_'));

    // initialize the history display
    historyDisplay.resize(16, "");

    board->addObserver(this);
}

void TextUI::updateBoardDisplay(MoveInfo latest) {
    
    // update board display based on board
    for (int i = 0; i < board->getSize(); ++i) {
        for (int j = 0; j < board->getSize(); ++j) {
            const Piece* piece = board->pieceAtSquare(i, j);
            if (!piece) {
                boardDisplay[i][j] = '_'; // empty square
                continue;
            }

            // piece type
            char pieceChar = firstChar(piece->getType());
            
            // adjust for player
            if (piece->getColour() == Colour::Black) {
                pieceChar = tolower(pieceChar);
            }

            boardDisplay[i][j] = pieceChar;
        }
    }
}

void TextUI::updateHistoryDisplay() {
    // clear the history display
    historyDisplay.clear();
    
    // get the latest moves from gameData
    vector<MoveInfo> moves = gameData->latestMoves();

    // update history display with the latest moves
    int tabsInLastLine = gameData->gameLength() % historyWidth;
    if (tabsInLastLine == 0) {tabsInLastLine = historyWidth;}
    int firstMoveRecorded = max(0, (int)moves.size() - (historyWidth * (historyDepth - 1)) - tabsInLastLine);

    for (int i=firstMoveRecorded; i < moves.size(); ++i) {
        historyDisplay.push_back(moves[i].algebraic());
    }
}

//in future updates (especially when we add rook movement info to moveinfo), we can optimize this
//to only redraw the parts of the board that have changed
//but for now, this is sufficient (despite not being optimal)

void TextUI::moveUpdate(MoveInfo latest) {
    
    updateBoardDisplay(latest);
    updateHistoryDisplay();

    // output!
    output();
}

void TextUI::undoUpdate(MoveInfo latest) {
    
    updateBoardDisplay(latest);
    updateHistoryDisplay();

    // output!
    output();
}

void TextUI::setupUpdate(MoveInfo latest) {
    
    updateBoardDisplay(latest);
    //setup updates do not affect history, so we do not update historyDisplay

    // output!
    output();
}



void TextUI::output() {

    cout << endl;
    int step = 0;

    // print from white
    for (int rank = board->getSize() - 1; rank >= 0; --rank) {
        cout << rank + 1 << " "; // numbers
        for (int file = 0; file < board->getSize(); ++file) {
            cout << boardDisplay[file][rank] << " ";
        }
        cout << "\t\t"; // tab for history display
        for (int i = 0; i < historyWidth && step < historyDisplay.size(); ++i) {
            cout << historyDisplay[step] << " ";
            ++step;
        }
        cout << endl;
    }
    cout << endl << "  ";
    for (int file = 0; file < board->getSize(); ++file) {
        cout << char('a' + file) << ' '; // letters
    }
    cout << '\n' << endl;

}



