#include "bot1.h"
#include "board.h"
#include "piece.h"
#include <random>

Bot1::Bot1(Board* board, Colour colour) : Bot(board, colour) {}

MoveInfo &Bot1::makeMove() {
    // This method should implement the logic for the bot's move
    // For example, it could randomly select a valid move from the available pieces.

    std::vector<MoveInfo> validMoves = board->getValidMoves(getColour());

    if (validMoves.empty()) {
        throw std::runtime_error("No valid moves available for Bot1.");
    }

    // Randomly select one of the valid moves
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validMoves.size() - 1);

    MoveInfo selectedMove = validMoves[dis(gen)];
    
    // Create and return a MoveInfo object representing the move
    return selectedMove;
}



