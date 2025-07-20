#include "board.h"

Board::Board() : size{size} {
    board = new Piece**[size];
    for (int i = 0; i < size; ++i) {
        board[i] = new Piece*[size];
        for (int j = 0; j < size; ++j) {
            board[i][j] = nullptr;
        }
    }
    gameData = new GameData();
    
}

int Board::getSize() const {
    return size;
}






