#include "human.h"

Human::Human(Board* board, Colour colour) : Player(board, colour) {}

// whether the player can move without specifying a move
bool Human::autoMovable() { return false; }
