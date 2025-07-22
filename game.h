#pragma once
#include <string>
#include <memory>
#include "board.h"
#include "gamedata.h"
#include "player.h"
#include "textui.h"
using namespace std;

enum class Mode { Home, Game, Setup };

class Game {

    const int NUM_PLAYERS = 2;

    Board *board;
    GameData *gameData;
    TextUI *tUI;
    int playerTurn = 0;
    vector<unique_ptr<Player>> players;
    vector<int> scores;
    Mode mode;
    
    public:
    Game();
    void run();

    private:
    void play();
    void setup();
    void nextTurn();
    void printScores() const;

};


