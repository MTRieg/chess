
class Piece {
    public:
        enum class Type {
            Pawn,
            Knight,
            Bishop,
            Rook,
            Queen,
            King
        };

        enum class Color {
            White,
            Black
        };

        Piece(Type type, Color color);

        Type getType() const;
        Color getColor() const;
}

