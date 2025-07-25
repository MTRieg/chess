#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <limits>
#include "game.h"
#include "human.h"
#include "bot1.h"
#include "bot2.h"
#include "bot3.h"
#include "bot4.h"
#include "board.h"
#include "gamedata.h"
#include "piece.h"
#include "abstractui.h"
#include "textui.h"
#include "gui.h" // Add this line
using namespace std;

Game::Game(bool useGUI) {
    board = new Board(8);
    gameData = new GameData();
    board->addObserver(gameData);
    tUI = std::make_unique<TextUI>(board, gameData);
    if(useGUI) {
        try {
            gui = std::make_unique<GUI>(board, gameData);
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
            gui.reset(); // Deletes and sets to nullptr
        }
    }
    mode = Mode::Home;
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        scores.push_back(0);
    }
    players.resize(NUM_PLAYERS);
}

Game::~Game() {
    if (gui) {board->removeObserver(gui.get());}
    if (tUI) {board->removeObserver(tUI.get());}

    cout << "Final Scores:" << endl;
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        cout << "Player " << i + 1 << ": " << scores[i] << " points" << endl;
    }
    cout << "Program terminated." << endl;

    delete board;
    delete gameData;
}

void Game::run()  {
    bool customSetup = false;
    string cmd;

    while (true) {
        cout << "Enter command (game, setup, quit): " << endl;
        cin >> cmd;

        if(cin.eof()){
            cout << "reached end of inputted commands, terminating program" << endl;
            return;
        }
        if (cin.fail()) {
            cout << "Invalid command (run loop)" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (cmd == "game") {
            Colour turnColour = Colour::White;
            for (int i = 0; i < NUM_PLAYERS; ++i) {
                cout << "Enter player type for " << turnColour << " (human, computer[1-4]): ";
                string playerType;
                cin >> playerType;
                if (playerType == "human") {
                    players[i] = make_unique<Human>(board, turnColour);
                } else if (playerType == "computer[1]") {
                    players[i] = make_unique<Bot1>(board, turnColour);
                } else if (playerType == "computer[2]") {
                    players[i] = make_unique<Bot2>(board, turnColour);
                } else if (playerType == "computer[3]") {
                    players[i] = make_unique<Bot3>(board, turnColour);
                } else if (playerType == "computer[4]") {
                    players[i] = make_unique<Bot4>(board, turnColour);
                } else {
                    cout << "Invalid player type: " << playerType << endl;
                    i--;
                    continue;
                }
                ++turnColour;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush leftover newline
            if(!customSetup){
                board->init();
            }
            mode = Mode::Game;            

        } else if (cmd == "setup") {
            mode = Mode::Setup;
            setup();
            customSetup = true;

        } else if (cmd == "quit") {
            break;
        }

        while (mode == Mode::Game && !cin.eof()) {
            play();
        }

    }
}


void Game::setup(){
    Colour turnColour = Colour::White;
    
    
    while (true) {
        cout << "Enter setup command (+ <piece> <position>, - <position>, = <starting_colour>, done): " << endl;

        string cmd;
        getline(cin, cmd);
        if(cin.eof()){
            return;
        }
        istringstream iss{cmd};

        string command;
        iss >> command;

        // parse command
        if (iss.fail()) {
            cout << "Invalid command (read fail)" << endl;
        }

        if (command == "done") {
            board->leaveSetupMode(turnColour);
            break;
        }else if (command == "+"){
            string pieceType, positionStr;
            iss >> pieceType >> positionStr;
            if (pieceType.empty() || positionStr.empty()) {
                cout << "Invalid command format. Use 'add <piece> <position>'." << endl;
                continue;
            }
            Position position = {0,0};
            try{
                position = Position(positionStr);
            }catch (const std::invalid_argument& e) {
                cout << "Invalid position format: " << positionStr << endl;
                continue;
            }

            if (!position.inBounds(board->getSize())) {
                cout << "Position out of bounds: " << positionStr << endl;
                continue;
            }

            
            Colour pieceColour = Colour::White;

            if ('a' <= pieceType[0] && pieceType[0] <= 'z'){
                pieceType[0] = toupper(pieceType[0]);
                pieceColour = Colour::Black;
            }

            Piece::PieceType type;
            try{
                type = charToPieceType(pieceType[0]);
            }catch (const std::invalid_argument& e) {
                cout << "Invalid piece type: " << pieceType << endl;
                continue;
            }

            Piece *piece = createPiece(pieceColour, position, type, board);

            board->addPiece(piece);

            cout << "Added " << pieceType << " at " << positionStr << endl;
        }else if (command == "-"){
            string positionStr;
            iss >> positionStr;
            if (positionStr.empty()) {
                cout << "Invalid command format. Use 'remove <position>'." << endl;
                continue;
            }
            Position position = {0,0};
            try{
                position = Position(positionStr);
            }catch (const std::invalid_argument& e) {
                cout << "Invalid position format: " << positionStr << endl;
                continue;
            }

            if (!position.inBounds(board->getSize())) {
                cout << "Position out of bounds: " << positionStr << endl;
                continue;
            }

            const Piece *piece = board->pieceAtPosition(position);
            if (piece) {
                board->removePiece(position);
            } else {
                cout << "No piece at position: " << positionStr << endl;
            }
            cout << "Removed piece at " << positionStr << endl;
        }else if (command == "="){
            string colourStr;
            iss >> colourStr;
            if (colourStr == "white" || colourStr == "White") {
                turnColour = Colour::White;
            } else if (colourStr == "black" || colourStr == "Black") {
                turnColour = Colour::Black;
            } else {
                cout << "Invalid colour: " << colourStr << endl;
                continue;
            }
            cout << "Set starting colour to " << (turnColour == Colour::White ? "White" : "Black") << endl;
        }

    }
}


void Game::play() {
    cout << "Current turn: " << players[playerTurn]->getColour() << endl;
    cout << "Enter a command: resign, move with no args for computer move, \n"
         << "or move <from> <to> [<promotionType>]:" << endl;

    string cmd;
    getline(cin, cmd);
    if(cin.eof()){
        return;
    }
    istringstream iss{cmd};

    

    // parse command
    if (iss.fail()) {
        cout << "Invalid command (read fail)" << endl;
        mode = Mode::Home;
        return;
    }

    

    // resignation
    if (cmd == "resign") {
        cout << players[playerTurn]->getColour() << " resigned." << endl;
        mode = Mode::Home;
        nextTurn();
        ++scores[playerTurn];
        return;
    }

    // game
    if (cmd == "game") {
        cout << "You are already in a game." << endl;
        return;
    }

    // computer move
    if (cmd == "move") {
        if (!players[playerTurn]->autoMovable()) {
            cout << "It's your turn, the computer has already played!" << endl;
            return;
        } else {
            if(players[playerTurn] == nullptr){
                cout << "No player at this turn, cannot make move" << endl;
                return;
            };
            MoveInfo move = players[playerTurn]->makeMove();
            try {
                board->movePiece(move);
                nextTurn();
            } catch (const std::invalid_argument& e) {
                cout << "Invalid move: " << e.what() << endl;
                return;
            }
            cout << "computer move" << endl;
        }
        return;
    }

    // human move
    if (cmd.substr(0, 5) == "move ") {
        string discard, fromPos, toPos, promotionType;
        iss >> discard >> fromPos >> toPos >> promotionType;
        if (fromPos.empty() || toPos.empty()) {
            cout << "Invalid move format. Use 'move <from> <to>'." << endl;
            return;
        }

        Position oldPos = {0,0}, newPos = {0,0};
        try{
            oldPos = Position(fromPos);
            newPos = Position(toPos);
        }
        catch (const std::invalid_argument& e) {
            cout << "Invalid position format: " << endl;
            cerr << e.what() << endl;
            return;
        }
        Piece::PieceType promoType;

        if(promotionType.length() == 0){
            promoType = Piece::PieceType::Queen;
        }else{
            switch (promotionType[0]){
                case 'R':
                case 'r':
                promoType = Piece::PieceType::Rook;
                break;
                case 'B':
                case 'b':
                promoType = Piece::PieceType::Bishop;
                break;
                case 'N':
                case 'n':
                promoType = Piece::PieceType::Knight;
                break;
                default:
                promoType = Piece::PieceType::Queen;
            }
        }

        if(oldPos.File == 4 && oldPos.Rank == 2 && newPos.File == 4 && newPos.Rank == 3){
            oldPos.File = 4;
        }


        // try move
        MoveInfo move;
        try{
            move = board->moveInfo(oldPos, newPos, promoType);
        }catch (const std::invalid_argument& e) {
            cout << "Invalid move: " << e.what() << endl;
            return;
        }
        if(move.colour() == players[playerTurn]->getColour()){
                try{
                board->movePiece(move);
                nextTurn();
            }
            catch (const std::invalid_argument& e) {
                cout << "Invalid move: " << e.what() << endl;
                return;
            }
            
        }else{
            cout << "Not the correct player's turn" << endl;
        }

        // check endings
        if(board->checkmate()){
            cout << players[playerTurn]->getColour() << " was checkmated." << endl;
            mode = Mode::Home;
            nextTurn();
            ++scores[playerTurn];
            return;
        }
        if(board->stalemate()){
            cout << players[playerTurn]->getColour() << " was stalemated." << endl;
            mode = Mode::Home;
            nextTurn();
            for(auto& score : scores){
                score += 0.5; // reset scores
            }
            return;
        }
        
        return;

    }else if (cmd == "undo") {
        if (gameData->gameLength() > 0) {
            MoveInfo lastMove = gameData->latestMoves(1)[0];
            board->undoAndNotify(lastMove);
            cout << "Undid last move." << endl;
            previousTurn();
        } else {
            cout << "No moves to undo." << endl;
            return;
        }

    } else {
        cout << "Invalid command (else)" << endl;
    }

}



void Game::nextTurn() {
    playerTurn = (playerTurn + 1) % players.size();
}

void Game::previousTurn() {
        playerTurn = (playerTurn - 1 + players.size()) % players.size();
    }

void Game::printScores() const {
    cout << "Final Score:" << endl;
    for (int i = 0; i < players.size(); ++i) {
        cout << players[i]->getColour() << ": " << scores[i] << endl;
    }
}

