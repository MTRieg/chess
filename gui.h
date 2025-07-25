#pragma once
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include "abstractui.h"
#include "board.h"
#include "piece.h"

class GUI : public AbstractUI {

    static const int DEFAULT_WIDTH = 576;
    static const int DEFAULT_HEIGHT = 576;

    // X11 variables
    Display *d;
    Window w;
    int s, width, height;
    GC gc;
    unsigned long colours[5];


    void drawPiece(Piece::PieceType type, Position p, Colour c);
    void drawBackground();
    void drawBackground(Position p);

    void reloadByMoveInfo(const MoveInfo &move);

    public:
    GUI(Board* board, GameData* gameData, int width=DEFAULT_WIDTH, int height=DEFAULT_HEIGHT);
    ~GUI();

    enum { White=0, Black, Red, Green, Blue }; // Available colours

    int getWidth() const;
    int getHeight() const;

    // drawing methods
    void fillRectangle(int x, int y, int width, int height, unsigned long colour);
    void drawString(int x, int y, const std::string &msg, unsigned long colour);
    void redrawSquare(Position p);

    // abstract ui methods
    void moveUpdate(MoveInfo latest) override;
    void undoUpdate(MoveInfo latest) override;
    void setupUpdate(MoveInfo latest) override;
    void output() override;

};
