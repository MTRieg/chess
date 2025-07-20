#include "bot.h"
#include "board.h"
#include "piece.h"

Bot::Bot(Board* board, Colour colour) : Player(board, colour) {}

// whether the player can move without specifying a move
bool Bot::autoMovable() { return true; }

