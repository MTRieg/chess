#include <string>
#include "board.h"

using namespace std;

int main() {

    Board board;
    string cmd;
    bool inGame = false;
    
    while (true) {
        cin >> cmd;
        if (cin.fail()) break;

        if (cmd == "game") {
            string player1, player2;
            cin >> player1 >> player2;
            inGame = true;
        }





        while (inGame) {
            
        }
    }

}
