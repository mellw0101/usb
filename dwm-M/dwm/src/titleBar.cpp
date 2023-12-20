# include "titleBar.hpp"
# include "SafeConverter.hpp"
# include "dwmLog.hpp"
#include <X11/Xlib.h>
#include <cstring>



// Window createTitleBar ( Display* d, Window w, int width ) {
//     Window titleBar;
//     int height = 20;
//     unsigned int black = BlackPixel ( d, DefaultScreen ( d ) );
//     unsigned int white = WhitePixel ( d, DefaultScreen ( d ) );
//     GC gc = XCreateGC( d, w, 0, nullptr );
//     XSetForeground ( d, gc, black );
// 	XSetBackground ( d, gc, white );
//     XFontStruct *font = XLoadQueryFont ( d, "-misc-fixed-medium-r-semicondensed--0-0-75-75-c-0-iso8859-1" );
//     if ( ! font )
// 		dwmLog::WARNING ( "could not load font" );
//     else
//         XSetFont ( d, gc, font->fid );
//
//     titleBar = XCreateSimpleWindow ( d, w, 0, 0, width, height, 1, 0, 0 );
//     XMapWindow ( d, w );
//     return titleBar;
// }

void drawRedSquare(Display* dpy, Window window, int squareSize) {
    dwmLog::FUNC ( __func__ );

    // First, get the attributes of the window to find its dimensions
    XWindowAttributes wa;
    XGetWindowAttributes ( dpy, window, &wa );

    // Create a graphics context for drawing
    XGCValues values;
    values.foreground = XBlackPixel(dpy, DefaultScreen(dpy));  // Initial color (black)
    values.background = XWhitePixel(dpy, DefaultScreen(dpy));  // Background color
    GC gc = XCreateGC(dpy, window, GCForeground | GCBackground, &values);

    // Set the color to red
    XColor red;
    Colormap colormap = DefaultColormap(dpy, DefaultScreen(dpy));
    if (XAllocNamedColor(dpy, colormap, "red", &red, &red)) {
        XSetForeground(dpy, gc, red.pixel);

        // Calculate position to center the square in the window
        int x = (wa.width - squareSize) / 2;
        int y = (wa.height - squareSize) / 2;

        // Draw a square
        XFillRectangle(dpy, window, gc, x, y, squareSize, squareSize);

        // Free the allocated color
        XFreeColors(dpy, colormap, &red.pixel, 1, 0);
    } else {
        dwmLog::WARNING("Could not allocate red color");
    }

    // Cleanup
    XFreeGC(dpy, gc);

    dwmLog::ENDFUNC(__func__);
}

void drawTitleBar( Display* d, Window w,  const std::string& title ) {
    XGCValues values;
    GC gc = XCreateGC ( d, w, 0, &values );

    // Set the foreground color for the title
    XSetForeground( d, gc, XWhitePixel ( d, 0 ) );

    int title_length = static_cast<int> ( title.length() );
    // Draw the title text
    XDrawString( d, w, gc, 10, 10, title.c_str(), title_length );

    // Clean up
    XFreeGC(d, gc);
}

void handleTitleBarEvent(XEvent* event, Display* display) {
    // Example: Handle a mouse click event
    if (event->type == ButtonPress) {
        // Code to handle button press, like moving the window
    }
}

void applyDecorations(Window w, Display* dpy) {
    // Assuming borderpx, titleBarHeight, and other necessary variables are defined globally or passed to this function
    dwmLog::FUNC(__func__);
    dwmLog::INFO("STARTING");

    XWindowAttributes wa;
    if (!XGetWindowAttributes(dpy, w, &wa)) {
        dwmLog::WARNING("XGetWindowAttributes could not get attributes");
        return;
    }

    // Create title bar
    int titleBarHeight = 20;  // Set the desired height of the title bar
    Window titleBar = XCreateSimpleWindow(dpy, w, 0, -titleBarHeight, wa.width, titleBarHeight, 0, 0, 0);

    // Set event masks for the title bar
    XSelectInput(dpy, titleBar, ExposureMask | ButtonPressMask);

    // Map the title bar to make it visible
    XMapWindow(dpy, titleBar);

    // Create graphics context for drawing in the title bar
    XGCValues values;
    GC gc = XCreateGC(dpy, titleBar, 0, &values);

    // Set the foreground color for drawing
    XSetForeground(dpy, gc, XBlackPixel(dpy, DefaultScreen(dpy)));

    // Draw something in the title bar, e.g., the title text
    const char* title = "Window Title"; // Replace with dynamic title if needed
    XDrawString(dpy, titleBar, gc, 10, 15, title, strlen(title));

    // Release graphics context
    XFreeGC(dpy, gc);

    // Adjust main window position to account for title bar
    XMoveResizeWindow(dpy, w, wa.x, wa.y + titleBarHeight, wa.width, wa.height);

    dwmLog::ENDFUNC(__func__);
}


