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
using namespace std;

Game::Game() {
    board = new Board(8);
    gameData = new GameData();
    tUI = new TextUI(board, gameData);

    board->init();
    board->addObserver(gameData);
    //abstractUI creation automatically attaches the ui as an observer of board

    mode = Mode::Home;
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        scores.push_back(0);
    }
    players.resize(NUM_PLAYERS);
}

void Game::run()  {

    string cmd;

    while (true) {
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
            mode = Mode::Game;            

        } else if (cmd == "setup") {
            mode = Mode::Setup;
            setup();

        } else if (cmd == "quit") {
            break;
        }

        while (mode == Mode::Game && !cin.eof()) {
            play();
        }

    }
}



void Game::play() {
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
            cout << "computer move" << endl;
        }

    }

    // human move
    if (cmd.substr(0, 5) == "move ") {
        string discard, fromPos, toPos, promotionType;
        iss >> discard >> fromPos >> toPos >> promotionType;
        if (fromPos.empty() || toPos.empty()) {
            cout << "Invalid move format. Use 'move <from> <to>'." << endl;
            return;
        }

        Position oldPos = Position(fromPos);
        Position newPos = Position(toPos);
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
        MoveInfo move = board->moveInfo(oldPos, newPos, promoType);
        if(move.colour() == players[playerTurn]->getColour() && board->isValidMove(move)){
            board->movePiece(move);
            nextTurn();
        }else{
            cout << "Move is not valid in this position" << endl;
        }

        // check endings
        if(board->checkmate()){
            cout << players[playerTurn]->getColour() << " was checkmated." << endl;
            mode = Mode::Home;
            nextTurn();
            ++scores[playerTurn];
            return;
        }
        
        return;

    } else {
        cout << "Invalid command (else)" << endl;
    }

}

void Game::setup() {

}

void Game::nextTurn() {
    playerTurn = (playerTurn + 1) % players.size();
}

void Game::printScores() const {
    cout << "Final Score:" << endl;
    for (int i = 0; i < players.size(); ++i) {
        cout << players[i]->getColour() << ": " << scores[i] << endl;
    }
}

