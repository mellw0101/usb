// buttonpress.h

#ifndef BUTTONPRESS_H
#define BUTTONPRESS_H

#include <X11/Xlib.h>

// Forward declarations of types used
struct Arg;
struct Client;
struct Monitor;

// Function declarations
void focusMonitor(XButtonPressedEvent *ev);
unsigned int processBarClick(XButtonPressedEvent *ev);
unsigned int handleClientWindowClick(XButtonPressedEvent *ev);
void executeButtonFunction(unsigned int click, XButtonPressedEvent *ev);
void buttonpress(XEvent *e);

#endif // BUTTONPRESS_H
