#include <X11/Xlib.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

int main() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Error: Could not open X display." << std::endl;
        return 1;
    }

    int screen_num = DefaultScreen(display);
    int screen_width = DisplayWidth(display, screen_num);
    int screen_height = DisplayHeight(display, screen_num);

    Window root_window = RootWindow(display, screen_num);

    // Create a black window with no border
    Window window = XCreateSimpleWindow(display, root_window, 0, 0, 1, 1, 0, 0, 0);
    XMapWindow(display, window);

    // Request exclusive fullscreen mode
    XEvent xev;
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;  // _NET_WM_STATE_ADD
    xev.xclient.data.l[1] = wm_state;
    XSendEvent(display, DefaultRootWindow(display), False,
               SubstructureRedirectMask | SubstructureNotifyMask, &xev);

    // Resize the window to fullscreen
    XResizeWindow(display, window, screen_width, screen_height);

    XFlush(display);

    XEvent event;
    while (1) {
        XNextEvent(display, &event);
    }

    XCloseDisplay(display);
    return 0;
}
