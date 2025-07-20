#include <string>
#include "board.h"
#include "gamedata.h"
#include "player.h"

enum class Mode { Home, Game, Setup };

class Game {

    const int NUM_PLAYERS = 2;

    Board *board;
    GameData *gameData;
    int playerTurn = 0;
    vector<Player> players;
    vector<int> scores;
    Mode mode;
    
    public:
    Game() = default;
    void run();

    private:
    void play();
    void setup();
    void nextTurn();

};


