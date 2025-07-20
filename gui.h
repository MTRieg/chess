#pragma once
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include "abstractui.h"

class GUI : public AbstractUI {

    static const int DEFAULT_WIDTH = 1024;
    static const int DEFAULT_HEIGHT = 768;

    // X11 variables
    Display *d;
    Window w;
    int s, width, height;
    GC gc;
    unsigned long colours[5];

    public:
    GUI(Board* board, GameData* gameData, int width=DEFAULT_WIDTH, int height=DEFAULT_HEIGHT);
    ~GUI();

    enum { White=0, Black, Red, Green, Blue }; // Available colours

    int getWidth() const;
    int getHeight() const;

    // drawing methods
    void fillRectangle(int x, int y, int width, int height, int colour=Black);
    void drawString(int x, int y, const std::string &msg);

    // abstract ui methods
    void update(MoveInfo latest) override;
    void output() override;

};
