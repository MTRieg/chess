#include "bot2.h"
#include "board.h"
#include "piece.h"
#include <random>

Bot2::Bot2(Board* board, Colour colour) : Bot(board, colour) {}

MoveInfo Bot2::makeMove() {
    // This method should implement the logic for the bot's move
    // For example, it could randomly select a valid move from the available pieces.

    std::vector<MoveInfo> validMoves = board->getValidMoves(getColour());

    if (validMoves.empty()) {
        throw std::runtime_error("No valid moves available for Bot2.");
    }

    std::vector<MoveInfo> capturingMoves;
    for(auto move : validMoves) {
        if(move.capturedPiece != nullptr) {
            capturingMoves.push_back(move);
        }
    }


    std::random_device rd;
    std::mt19937 gen(rd());
    if(capturingMoves.empty()) {
        // If no capturing moves, select a random valid move
        std::uniform_int_distribution<> dis(0, validMoves.size() - 1);
        return validMoves[dis(gen)];
    }else{
        // If there are capturing moves, select one randomly
        std::uniform_int_distribution<> dis(0, capturingMoves.size() - 1);
        return capturingMoves[dis(gen)];
    }
}

