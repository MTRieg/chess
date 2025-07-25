#include <string>
#include "game.h"
#include "board.h"

using namespace std;

int main(int argc, char *argv[]) {

    bool useGUI = true;
    if (argc > 1) {
        string arg = argv[1];
        if (arg == "txt" || arg == "txtui") {
            useGUI = false;
        }
    }

    Game game = Game(useGUI);
    game.run();

}
