#include "bot3.h"
#include "board.h"
#include "piece.h"
#include <random>

Bot3::Bot3(Board* board, Colour colour) : Bot(board, colour) {}

MoveInfo Bot3::makeMove() {
    // This method should implement the logic for the bot's move
    // For example, it could randomly select a valid move from the available pieces.

    std::vector<MoveInfo> validMoves = board->getValidMoves(getColour());

    if (validMoves.empty()) {
        throw std::runtime_error("No valid moves available for Bot3.");
    }

    std::vector<MoveInfo> nonCapturingMoves;
    for(auto move : validMoves) {
        if(move.capturedPiece == nullptr) {
            nonCapturingMoves.push_back(move);
        }
    }


    std::random_device rd;
    std::mt19937 gen(rd());
    if(nonCapturingMoves.empty()) {
        // If no non-capturing moves, select a random valid move
        std::uniform_int_distribution<> dis(0, validMoves.size() - 1);
        return validMoves[dis(gen)];
    }else{
        // If there are non-capturing moves, select one randomly
        std::uniform_int_distribution<> dis(0, nonCapturingMoves.size() - 1);
        return nonCapturingMoves[dis(gen)];
    }
}


