#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "game.h"
#include "human.h"
#include "bot1.h"
#include "bot2.h"
#include "bot3.h"
#include "bot4.h"
#include "board.h"
#include "gamedata.h"
#include "piece.h"
using namespace std;

Game::Game() {
    board = new Board();
    gameData = new GameData();
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
        if (cin.fail()) {
            cout << "Invalid command" << endl;
            continue;
        }

        if (cmd == "game") {\
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
                    continue;
                }
                ++turnColour;
            }
            mode = Mode::Game;

            

        } else if (cmd == "setup") {
            mode = Mode::Setup;
            setup();

        } else if (cmd == "quit") {
            break;
        }

        while (mode == Mode::Game) {
           play();
        }

    }
}



void Game::play() {
    string cmd;
    getline(cin, cmd);
    istringstream iss{cmd};

    // parse command
    if (iss.fail()) {
        cout << "Invalid command" << endl;
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
    if (cmd.substr(0, 4) == "move ") {
        string fromPos, toPos, promotionType;
        iss >> fromPos >> toPos >> promotionType;
        if (fromPos.empty() || toPos.empty()) {
            cout << "Invalid move format. Use 'move <from> <to>'." << endl;
            return;
        }

        // try move
        

        // check endings
        
        return;

    } else {
        cout << "Invalid command" << endl;
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

