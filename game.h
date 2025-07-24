#pragma once
#include <string>
#include <memory>
#include "board.h"
#include "gamedata.h"
#include "player.h"
#include "textui.h"
#include "gui.h"
using namespace std;

enum class Mode { Home, Game, Setup };

class Game {

    const int NUM_PLAYERS = 2;

    Board *board;
    GameData *gameData;
    std::unique_ptr<TextUI> tUI;
    std::unique_ptr<GUI> gui;
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


