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

    // default setup
    turnColour = Piece::Colour::White;
    players = { Player::Human, Player::Computer1 };
    scores = {0, 0};
}

void Game::run()  {

    string cmd;

    while (true) {
        cin >> cmd;
        if (cin.fail()) {
            cout << "Invalid command" << endl;
            continue;
        }

        if (cmd == "game") {
            string player1, player2;
            cin >> player1 >> player2;
            switch (player1) {
                case "human": player1 = Player::Human; break;
                case "computer[1]": player1 = Player::Computer1; break;
                case "computer[2]": player1 = Player::Computer2; break;
                case "computer[3]": player1 = Player::Computer3; break;
                case "computer[4]": player1 = Player::Computer4; break;
                default: cout << "Invalid player type" << endl; continue;
            }
            switch (player2) {
                case "human": player2 = Player::Human; break;
                case "computer[1]": player2 = Player::Computer1; break;
                case "computer[2]": player2 = Player::Computer2; break;
                case "computer[3]": player2 = Player::Computer3; break;
                case "computer[4]": player2 = Player::Computer4; break;
                default: cout << "Invalid player type" << endl; continue;
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
        cout << players[playerTurn] << " resigns." << endl;
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
        cout << "Computer is thinking..." << endl;

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



