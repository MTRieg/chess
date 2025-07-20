#include <vector>
#include <string>
#include "abstractui.h"
#include "board.h"
#include "gamedata.h"
using namespace std;

class TextUI : public AbstractUI {

    vector<vector<char>> boardDisplay;
    vector<string> historyDisplay;
    int historyWidth;

    public:
    TextUI(Board* board, GameData* gameData) : AbstractUI(board, gameData) {}

    void update(MoveInfo latest);

    private:
    void output();

};
