#include "piece.h"

Piece::Piece(Type type, Color color) : type(type), color(color) {}

Type Piece::getType() const { return type; }
Color Piece::getColor() const { return color; }
