#ifndef TITLEBAR_HPP
#define TITLEBAR_HPP

# include <X11/X.h>
#include <X11/Xlib.h>
#include <string>

// Window createTitleBar ( Display* d, Window w, int width );
void drawRedSquare(Display* dpy, Window window, int squareSize);
void drawTitleBar( Display* d, Window w,  const std::string& title );
void handleTitleBarEvent(XEvent* event, Display* display);
void applyDecorations(Window w, Display* dpy);

#endif
