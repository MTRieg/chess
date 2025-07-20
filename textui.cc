#include "textui.h"
#include "piece.h"

TextUI::TextUI(Board* board, GameData* gameData)
    : AbstractUI(board, gameData) {

    // initialize the board display based on size
    boardDisplay.resize(board->getSize(), vector<char>(board->getSize(), '_'));

    // initialize the history display
    historyDisplay.resize(16, "");
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
                    pieceChar = 'P';
                    break;
                case Piece::PieceType::Knight:
                    pieceChar = 'N';
                    break;
                case Piece::PieceType::Bishop:
                    pieceChar = 'B';
                    break;
                case Piece::PieceType::Rook:
                    pieceChar = 'R';
                    break;
                case Piece::PieceType::Queen:
                    pieceChar = 'Q';
                    break;
                case Piece::PieceType::King:
                    pieceChar = 'K';
                    break;
            }
            
            // adjust for player
            if (piece->getColour() == Colour::Black) {
                pieceChar = tolower(pieceChar);
            }

            boardDisplay[i][j] = pieceChar;
        }
    }

    // update history display
    historyDisplay.clear();
    
    vector<MoveInfo> moves = gameData->latestMoves();

    for(int i = turn; i < moves.size(); ++i) {
        historyDisplay.push_back(moves[i].algebraic());
    }

    // output!
    output();
}

void TextUI::output() {

    system("clear"); // clear the console
    int step = 0;

    // print from white's perspective
    for (int rank = 0; rank < board->getSize(); ++rank) {
        cout << 8 - rank << " "; // numbers
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
        cout << char('a' + file); // letters
    }
    cout << endl;

}



