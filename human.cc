#include "human.h"

Human::Human(Board* board, Colour colour) : Player(board, colour) {}

// whether the player can move without specifying a move
bool Human::autoMovable() { return false; }

MoveInfo &Human::makeMove() {
    // This method should implement the logic for human player interaction
    // For example, it could read input from the console to determine the move
    // and return a MoveInfo object representing that move.
    
    // Placeholder implementation:
    throw std::runtime_error("Human player interaction should not occur through these functions.");
}
