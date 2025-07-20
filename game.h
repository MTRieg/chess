#include <string>
#include "board.h"

enum class Mode { Home, Game, Setup };
enum class Player { Human, Computer1, Computer2, Computer3, Computer4 };

class Game {

    Board board;
    GameData gameData;
    Piece::Colour turnColour;
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
