#include <iostream>
#include <sstream>
#include <string>
#include "game.h"
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
            Colour turnColour = Piece::Colour::White;
            for (int i = 0; i < NUM_PLAYERS; ++i) {
                string playerType;
                cin >> playerType;
                if (playerType == "human") {
                    players[i] = new Human(board, turnColour++);
                } else if (playerType == "computer[1]") {
                    player = Player::Computer1;
                } else if (playerType == "computer[2]") {
                    player = Player::Computer2;
                } else if (playerType == "computer[3]") {
                    player = Player::Computer3;
                } else if (playerType == "computer[4]") {
                    player = Player::Computer4;
                } else {
                    cout << "Invalid player type: " << playerType << endl;
                    continue;
                }
            }
            mode = Mode::Game;

        } else if (cmd == "setup") {
            mode = Mode::Setup;
            // board

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
        cout << "Player " << playerTurn << " resigned." << endl;
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
        if (players[playerTurn] == Player::Human) {
            cout << "The computer has already played." << endl;
            return;
        }
        if (players[playerTurn] == Player::Computer1) {

            cout << "Computer 1 is making a move..." << endl;
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
    turnColour = (turnColour == Piece::Colour::White) ? Piece::Colour::Black : Piece::Colour::White;
    playerTurn = (playerTurn + 1) % players.size();
}


