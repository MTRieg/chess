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
        cerr << "Cannot open display" << endl;
        exit(1);
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
    char color_vals[5][10] = {"white", "black", "red", "green", "blue"};

    cmap = DefaultColormap(d, DefaultScreen(d));
    for (int i = 0; i < 5; ++i) {
        XParseColor(d, cmap, color_vals[i], &xcolour);
        XAllocColor(d, cmap, &xcolour);
        colours[i] = xcolour.pixel;
    }

    XSetForeground(d, gc, colours[Black]);

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
    hints.height = hints.base_height = hints.min_height = hints.max_height = height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints(d, w, &hints);

    XSynchronize(d, True);

    usleep(1000);
}

GUI::~GUI() {
    XFreeGC(d, gc);
    XCloseDisplay(d);
}

// accessors
int GUI::getWidth() const { return width; }
int GUI::getHeight() const { return height; }

// drawing methods
void GUI::fillRectangle(int x, int y, int width, int height, int colour) {
    XSetForeground(d, gc, colours[colour]);
    XFillRectangle(d, w, gc, x, y, width, height);
}

void GUI::drawString(int x, int y, const std::string &msg) {
    XSetForeground(d, gc, colours[White]);
    XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
}

// output methods
void GUI::update(MoveInfo latest) {
    output();
}


