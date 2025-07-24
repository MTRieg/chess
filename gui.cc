#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "gui.h"
using namespace std;

GUI::GUI(Board* board, GameData* gameData, int width, int height)
    : AbstractUI{board, gameData}, width{width}, height{height} {

    d = XOpenDisplay(NULL);
    if (d == NULL) {
        cerr << "GUI: Cannot open display" << endl;
        throw runtime_error("Cannot open display");
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapRaised(d, w);

    gc = XCreateGC(d, w, 0, (XGCValues *)0);

    // Set up colours.
    XColor xcolour;
    Colormap cmap;
    // 0: light square, 1: dark square, 2: white piece, 3: black piece
    const char* color_vals[4] = {"#F0D9B5", "#B58863", "#FFFFFF", "#000000"};

    cmap = DefaultColormap(d, DefaultScreen(d));
    for (int i = 0; i < 4; ++i) {
        XParseColor(d, cmap, color_vals[i], &xcolour);
        XAllocColor(d, cmap, &xcolour);
        colours[i] = xcolour.pixel;
    }

    XSetForeground(d, gc, colours[3]);

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
    hints.height = hints.base_height = hints.min_height = hints.max_height = height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints(d, w, &hints);

    XSynchronize(d, True);

    usleep(1000);

    board->addObserver(this);
}

GUI::~GUI() {
    XFreeGC(d, gc);
    XCloseDisplay(d);
}

// accessors
int GUI::getWidth() const { return width; }
int GUI::getHeight() const { return height; }

// drawing methods
void GUI::fillRectangle(int x, int y, int width, int height, unsigned long colour) {
    XSetForeground(d, gc, colour);
    XFillRectangle(d, w, gc, x, y, width, height);
}

void GUI::drawString(int x, int y, const std::string &msg, unsigned long colour) {
    XSetForeground(d, gc, colour);
    XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
}

// output methods
void GUI::update(MoveInfo latest) {
    output();
}

void GUI::drawBackground() {
    for (int i = 0; i < board->getSize(); ++i) {
        for (int j = 0; j < board->getSize(); ++j) {
            fillRectangle(i * 64, j * 64, 64, 64, (i + j) % 2 == 0 ? colours[0] : colours[1]);
        }
    }
}


void GUI::output(){
    drawBackground();
    for(int i=0; i < board->getSize(); ++i) {
        for(int j=0; j < board->getSize(); ++j) {
            Position pos(i, j);
            const Piece *piece = board->pieceAtPosition(pos);
            if (piece) {
                drawPiece(piece->getType(), pos, piece->getColour());
            } 
        }
    }
}


void GUI::drawPiece(Piece::PieceType type, Position p, Colour c) {
    char pieceSymbol = firstChar(type);
    int x = p.File * 64 + 32; 
    int y = p.Rank * 64 + 32; 
    // 2: white piece, 3: black piece
    int pieceColour = (c == Colour::White) ? 2 : 3;
    string pieceSymbolStr(1, pieceSymbol);
    drawString(x, y, pieceSymbolStr, colours[pieceColour]);
}




