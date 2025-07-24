#include "textui.h"
#include "piece.h"

TextUI::TextUI(Board* board, GameData* gameData)
    : AbstractUI(board, gameData) {

    // initialize the board display based on size
    boardDisplay.resize(board->getSize(), vector<char>(board->getSize(), '_'));

    // initialize the history display
    historyDisplay.resize(16, "");

    board->addObserver(this);
}

void TextUI::update(MoveInfo latest) {
    
    // update board display based on board
    for (int i = 0; i < board->getSize(); ++i) {
        for (int j = 0; j < board->getSize(); ++j) {
            const Piece* piece = board->pieceAtSquare(i, j);
            if (!piece) {
                boardDisplay[i][j] = '_'; // empty square
                continue;
            }

            // piece type
            char pieceChar;
            switch (piece->getType()) {
                case Piece::PieceType::Pawn:
                    pieceChar = 'p';
                    break;
                case Piece::PieceType::Knight:
                    pieceChar = 'n';
                    break;
                case Piece::PieceType::Bishop:
                    pieceChar = 'b';
                    break;
                case Piece::PieceType::Rook:
                    pieceChar = 'r';
                    break;
                case Piece::PieceType::Queen:
                    pieceChar = 'q';
                    break;
                case Piece::PieceType::King:
                    pieceChar = 'k';
                    break;
            }
            
            // adjust for player
            if (piece->getColour() == Colour::Black) {
                pieceChar = toupper(pieceChar);
            }

            boardDisplay[i][j] = pieceChar;
        }
    }

    // update history display
    historyDisplay.clear();
    int turn = gameData->gameLength() % 2; // get the current turn number
    vector<MoveInfo> moves = gameData->latestMoves();

    for(int i = turn; i < moves.size(); ++i) {
        historyDisplay.push_back(moves[i].algebraic());
    }

    // output!
    output();
}

void TextUI::output() {

    //system("clear"); // clear the console
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
    cout << endl;

}



