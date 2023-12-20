/* dwm.h */

#ifndef DWM_H
#define DWM_H

struct Arg;
struct Client;
struct Monitor;

#include "src/dwmLog.hpp"
#include <X11/XKBlib.h>// Make sure to include this header
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <cerrno>
#include <clocale>
#include <cmath>
#include <csignal>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include <X11/Xft/Xft.h>

#include "drw.hpp"
#include "util.hpp"

#	ifndef FUNCTION_DECLARATIONS
#define FUNCTION_DECLARATIONS

/* function declarations */
static void applyrules ( Client *c );
static int applysizehints ( Client *c, int *x, int *y, int *w, int *h,
							int interact );
static void arrange ( Monitor *m );
static void arrangemon ( Monitor *m );
static void attach ( Client *c );
static void attachstack ( Client *c );
static void buttonpress ( XEvent *e );
static void checkotherwm ();
static void cleanup ();
static void cleanupmon ( Monitor *mon );
static void clientmessage ( XEvent *e );
static void configure ( Client *c );
static void configurenotify ( XEvent *e );
static void configurerequest ( XEvent *e );
static Monitor *createmon ();
static void destroynotify ( XEvent *e );
static void detach ( Client *c );
static void detachstack ( Client *c );
static void drawbar ( Monitor *m );
static void drawbars ();
static void enternotify ( XEvent *e );
static void expose ( XEvent *e );
static void focus ( Client *c );
static void focusin ( XEvent *e );
static void focusstack ( const Arg *arg );
static Atom getatomprop ( Client *c, Atom prop );
static int getrootptr ( int *x, int *y );
static long getstate ( Window w );
static int gettextprop ( Window w, Atom atom, char *text, unsigned int size );
static void grabbuttons ( Client *c, int focused );
static void grabkeys ();
static void keypress ( XEvent *e );
static void killclient ( const Arg *arg );
static void manage ( Window w, XWindowAttributes *wa );
static void mappingnotify ( XEvent *e );
static void maprequest ( XEvent *e );
static void motionnotify ( XEvent *e );
static void movemouse ( const Arg *arg );
static void propertynotify ( XEvent *e );
// static void quit(const Arg *arg);
static Monitor *recttomon ( int x, int y, int w, int h );
static void resize ( Client *c, int x, int y, int w, int h, int interact );
static void resizeclient ( Client *c, int x, int y, int w, int h );
static void resizemouse ( const Arg *arg );
static void restack ( Monitor *m );
static void run ();
static void scan ();
static int sendevent ( Client *c, Atom proto );
static void sendmon ( Client *c, Monitor *m );
static void setclientstate ( Client *c, long state );
static void setfocus ( Client *c );
static void setfullscreen ( Client *c, int fullscreen );
static void setup ();
static void seturgent ( Client *c, int urg );
static void showhide ( Client *c );
static void spawn ( const Arg *arg );
static void togglebar ( const Arg *arg );
static void togglefloating ( const Arg *arg );
static void unfocus ( Client *c, int setfocus );
static void unmanage ( Client *c, int destroyed );
static void unmapnotify ( XEvent *e );
static void updatebarpos ( Monitor *m );
static void updatebars ();
static void updateclientlist ();
static int updategeom ();
static void updatenumlockmask ();
static void updatesizehints ( Client *c );
static void updatestatus ();
static void updatetitle ( Client *c );
static void updatewindowtype ( Client *c );
static void updatewmhints ( Client *c );
static void view ( const Arg *arg );
static Client *wintoclient ( Window w );
static Monitor *wintomon ( Window w );
static int xerror ( Display *dpy, XErrorEvent *ee );
static int xerrordummy ( Display *dpy, XErrorEvent *ee );
static int xerrorstart ( Display *dpy, XErrorEvent *ee );



#	endif /* FUNCTION_DECLARATIONS */


#endif /* DWM_H */
