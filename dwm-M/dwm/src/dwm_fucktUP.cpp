/* See LICENSE file for copyright and license details.
 *
 * dynamic window manager is designed like any other X client as well. It is
 * driven through handling X events. In contrast to other X clients, a window
 * manager selects for SubstructureRedirectMask on the root window, to receive
 * events about window (dis-)appearance. Only one X connection at a time is
 * allowed to select for this event mask.
 *
 * The event handlers of dwm are organized in an array which is accessed
 * whenever a new event has been fetched. This allows event dispatching
 * in O(1) time.
 *
 * Each child of the root window is called a client, except windows which have
 * set the override_redirect flag. Clients are organized in a linked client
 * list on each monitor, the focus history is remembered through a stack list
 * on each monitor. Each client contains a bit array to indicate the tags of a
 * client.
 *
 * Keys and tagging rules are organized as arrays and defined in config.h.
 *
 * To understand everything else, start reading main().
*/

#include "src/dwmLog.hpp"
#include <X11/XKBlib.h>	// Make sure to include this header
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
#include <unordered_map>

#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif /* XINERAMA */
#include <X11/Xft/Xft.h>

#include "drw.hpp"
#include "util.hpp"

/* macros */
#define BUTTONMASK (ButtonPressMask | ButtonReleaseMask)
#define CLEANMASK(mask) (mask & ~(numlockmask | LockMask) & (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask))
#define INTERSECT(x, y, w, h, m)                                                         \
	(MAX(0, MIN((x) + (w), (m)->wx + (m)->ww) - MAX((x), (m)->wx)) * MAX(0, MIN((y) + (h), (m)->wy + (m)->wh) - MAX((y), (m)->wy)))
#define ISVISIBLE(C) ((C->tags & C->mon->tagset[C->mon->seltags]))
#define LENGTH(X) (sizeof X / sizeof X[0])
#define MOUSEMASK (BUTTONMASK | PointerMotionMask)
// #define WIDTH(X) ((X)->w + 2 * (X)->bw)
#define WIDTH(X) ((X)->w + 2)
// #define HEIGHT(X) ((X)->h + 2 * (X)->bw)
#define HEIGHT(X) ((X)->h + 2)
#define TAGMASK ((1 << LENGTH(tags)) - 1)
#define TEXTW(X) (drw_fontset_getwidth (drw, (X)) + lrpad)

#ifndef ENUMS
#define ENUMS
enum {
	CurNormal,
	CurResize,
	CurMove,
	CurLast
}; /* cursor */
enum {
	NetSupported,
	NetWMName,
	NetWMState,
	NetWMCheck,
	NetWMFullscreen,
	NetActiveWindow,
	NetWMWindowType,
	NetWMWindowTypeDialog,
	NetClientList,
	NetLast
}; /* EWMH atoms */
enum {
	WMProtocols,
	WMDelete,
	WMState,
	WMTakeFocus,
	WMLast
}; /* default atoms */
enum {
	ClkTagBar,
	ClkLtSymbol,
	ClkStatusText,
	ClkWinTitle,
	ClkClientWin,
	ClkRootWin,
	ClkLast,
	WmClose,
	WmMove
}; /* clicks */
enum {
	CLOSE,
	MAXIMIZE,
	MOVE
}; /* window decoratios */
#endif/*ENUMS*/
typedef union {
	int i;
	unsigned int ui;
	float f;
	const void *v;
} Arg;
typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void ( *func ) ( const Arg *arg );
	const Arg arg;
} Button;
typedef class Monitor Monitor;
typedef class Client Client;
struct TitleBar {
    Window win; // The window for the title bar
    int h, w;
    std::string title;
    GC gc;
};
typedef struct {
	unsigned int mod;
	KeySym keysym;
	void ( *func ) ( const Arg * );
	const Arg arg;
} Key;
typedef struct {
	const char *symbol;
	void ( *arrange ) ( Monitor * );
} Layout;
typedef struct {
	const char *class_name;
	const char *instance;
	const char *title;
	unsigned int tags;
	int isfloating;
	int monitor;
} Rule;
#ifndef FUNCTION_DECLARATIONS
#define FUNCTION_DECLARATIONS

/* function declarations */
static void applyrules ( Client *c );
static int applysizehints ( Client *c, const int *x, const int *y, int *w, int *h, int interact );
static void arrange ( Monitor *m );
static void arrangemon ( Monitor *m );
static void buttonpress ( XEvent *e );
static void checkotherwm ();
static void cleanup ();
static void cleanupmon ( Monitor *mon );
static Monitor *createmon ();
static void drawbar ( Monitor *m );
static void drawbars ();
static void focusstack ( const Arg *arg );
static int getrootptr ( int *x, int *y );
static long getstate ( Window w );
static int gettextprop ( Window w, Atom atom, char *text, unsigned int size );
static void grabkeys ();
static void killclient ( const Arg *arg );
// static void manage ( Window w, XWindowAttributes *wa );
static void movemouse ( const Arg *arg );
static Monitor *recttomon ( int x, int y, int w, int h );
static void resizemouse ( const Arg *arg );
static void restack ( Monitor *m );
static void run ();
static void scan ();
static void sendmon ( Client *c, Monitor *m );
static void setup ();
static void spawn ( const Arg *arg );
static void togglebar ( const Arg *arg );
static void togglefloating ( const Arg *arg );
static void updatebarpos ( Monitor *m );
static void updatebars ();
static void updateclientlist ();
static int updategeom ();
static void updatenumlockmask ();
static void updatestatus ();
static void view ( const Arg *arg );
static Client *wintoclient ( Window w );
static Monitor *wintomon ( Window w );
static int xerror ( Display *dpy, XErrorEvent *ee );
static int xerrordummy ( Display *dpy, XErrorEvent *ee );
static int xerrorstart ( Display *dpy, XErrorEvent *ee );


static void maprequest(XEvent *e);


#endif/*FUNCTION_DECLARATIONS*/
#ifndef M_CUSTOM
#define M_CUSTOM
#include "SafeConverter.hpp"
static void shiftview ( const Arg *arg );
static void shiftview_app ( const Arg *arg );
static void view_with_client ( const Arg *arg );
static void QUIT ( const Arg *arg );
static void applyWinDecorations ( Client* c );
#endif/*M_CUSTOM*/
#ifndef variables
#define variables
static const char broken[] = "broken";
static char stext[ 256 ];
static int screen;
static int sw, sh; /* X display screen geometry width, height */
static int bh;     /* bar height */
static int lrpad;  /* sum of left and right padding for text */
static int ( *xerrorxlib ) ( Display *, XErrorEvent * );
static unsigned int numlockmask = 0;
static Atom wmatom[ WMLast ], netatom[ NetLast ];
static int running = 1;
static Cur *cursor[ CurLast ];
static Clr **scheme;
static Display *dpy;
static Drw *drw;
static Monitor *mons, *selmon;
static Window root, wmcheckwin;
#endif/*variables*/
#ifndef CONFIG
#define CONFIG
#include <X11/X.h>
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 0;            /* 0 means bottom bar */
static const char *fonts[] = { "monospace:size=12" };
enum ColorSchemeIndex {
	SchemeNorm,
	SchemeSel
};
struct ColorScheme {
	const char *fg;
	const char *bg;
	const char *border;
};
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_cyan[]  = "#005577";
static const ColorScheme colors[] = {
	{ col_gray3, col_gray1, col_gray2 },	/* SchemeNorm */
	{ col_gray4, col_cyan, 	col_cyan  },	/* SchemeSel  */
};
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* 	class      		instance    title      	tags mask 	isfloating	monitor */
	{ 	nullptr, 		nullptr, 	nullptr, 	0, 			1, 			-1 		},
	{ 	"Gimp", 		nullptr, 	nullptr, 	0, 			1, 			-1 		},
};

/* layout(s) */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
static const Layout layouts[] = {
	/* no layout function means floating behavior */
	{ "><>", 	nullptr },
/*	{ "[]=", 	tile 	},    first entry is default */
/*	{ "[M]", 	monocle },*/
};
#ifndef KEY_DEFENITIONS
#define KEY_DEFENITIONS
#ifndef KEYS
#define KEYS
#define ALT Mod1Mask
#define SUPER Mod4Mask
#define CTRL ControlMask
#define SHIFT ShiftMask
#define TAB XK_Tab
#define L XK_Left
#define R XK_Right
#define T XK_t
#define Q XK_q
#define C XK_c
#define B XK_b
#endif/*KEYS*/
#ifndef TAGKEYS
#define TAGKEYS( KEY, TAG )	\
	{ ALT, KEY, 				view, 		{ .ui = 1 << ( TAG ) } },   \
	{ ALT | CTRL, KEY, 			toggleview, { .ui = 1 << ( TAG ) } },	\
	{ ALT | SHIFT, KEY, 		tag, 		{ .ui = 1 << ( TAG ) } },   \
	{ ALT | CTRL | SHIFT, KEY, 	toggletag, 	{ .ui = 1 << ( TAG ) } },
#endif/*TAGKEYS*/
#ifndef SHIFTVIEW
#define SHIFTVIEW( KEY, TAG )	\
	{ SUPER | CTRL, KEY, 			shiftview, 		{ .i = ( TAG ) } }, \
	{ SUPER | SHIFT | CTRL, KEY, 	shiftview_app, 	{ .i = ( TAG ) } },
#endif/*SHIFTVIEW*/
#ifndef SPAWN
#define SPAWN( MOD, KEY, TAG ) \
	{ MOD, KEY, spawn, { .v = ( TAG ) } },
#endif/*SPAWN*/
#endif/*KEY_DEFENITIONS*/
#ifndef COMMANDS
#define COMMANDS
// static const char *dwm_KILL[] 	= 	{ "/usr/bin/dwm-KILL", nullptr };
static const char *alacritty[] 	= 	{ "/usr/bin/alacritty", nullptr };
#endif/*COMMANDS*/
static const Key keys[] = {			/* KEY combos and what thay do */
	/*	modifier		key		function  	argument 	*/
	{ 	ALT, 			B, 		togglebar, 	{ 0 } 		},
	{ 	ALT | SHIFT, 	C, 		killclient, { 0 } 		},
	{ 	ALT, 			TAB, 	focusstack, { .i = +1 } },
	{	ALT | SHIFT, 	Q, 		QUIT, 		{ 0 }		},
	SPAWN
	( 	ALT | CTRL, 	T,	 				alacritty 	)
	SHIFTVIEW( 			L, 					-1 			)
	SHIFTVIEW( 			R, 					+1 			)
};
static const Button buttons[] = {	/* button definitions */
		/*	click			event mask		button 		function  		argument 	*/
		{ 	ClkClientWin, 	ALT, 			Button1, 	movemouse, 		{ 0 } 		},
		{ 	ClkClientWin, 	ALT, 			Button3, 	resizemouse, 	{ 0 } 		},
		{ 	WmMove, 		0, 				Button1, 	nullptr, 		{ 0 } 		},	/* Needed to make window moveble by titlebar */
};
struct NumTags {					/* compile-time check if all tags fit into an unsigned int bit array. */
	char limitexceeded[ LENGTH ( tags ) > 31 ? -1 : 1 ];
};
#endif/*CONFIG*/
class Monitor {
public:
	char ltsymbol[ 16 ];
	int num;
	int by;            // bar geometry
	int mx, my, mw, mh;// screen size
	int wx, wy, ww, wh;// window area
	unsigned int seltags;
	unsigned int sellt;
	unsigned int tagset[ 2 ];
	int showbar;
	int topbar;
	Client *clients;
	Client *sel;
	Client *stack;
	Monitor *next;
	Window barwin;
	const Layout *lt[ 2 ];
};
class Client {
public:
    // Constructors
    Client() = default;
    Client(const Client&) = default;
    Client(Client&&) = default;

    // Assignment Operators
    Client& operator=(const Client&) = default;
    Client& operator=(Client&&) = default;

    // Public member variables
    char name[256]{};
    int x{}, y{}, w{}, h{};
    int basew{}, baseh{}, incw{}, inch{}, maxw{}, maxh{}, minw{}, minh{}, hintsvalid{};
    int isfixed{}, isfloating{}, isurgent{}, neverfocus{}, oldstate{}, isfullscreen{};
    unsigned int tags{};
    Window win{};
    Client *next{};
    Client *snext{};
    Monitor *mon{};
    TitleBar tB;

	static void attach(Client *c) {
		c->next = c->mon->clients;
		c->mon->clients = c;
	}
	static void attachstack(Client *c) {
		c->snext = c->mon->stack;
		c->mon->stack = c;
	}
	static void detach(Client *c) {
		if (! c || ! c->mon) { return; }
		Client **tc;
		for (tc = &c->mon->clients; *tc && *tc != c; tc = &(*tc)->next) {}
		*tc = c->next;
	}
	static void detachstack(Client *c) {
		if (! c || ! c->mon) { return; }
		Client **tc, *t;

		for (tc = &c->mon->stack; *tc && *tc != c; tc = &(*tc)->snext) {}
		*tc = c->snext;

		if (c == c->mon->sel) {
			for (t = c->mon->stack; t && ! ISVISIBLE(t); t = t->snext) {}
			c->mon->sel = t;
		}
	}
	static void configure(Client *c) {
		// Validation
		if (!c || !dpy) { return; }

		XConfigureEvent ce = {0};
		ce.type = ConfigureNotify;
		ce.display = dpy;
		ce.event = c->win;
		ce.window = c->win;
		ce.x = c->x;
		ce.y = c->y;
		ce.width = c->w;
		ce.height = c->h;
		// ce.border_width = c->bw;
		ce.above = None;
		ce.override_redirect = False;

		Status status = XSendEvent(dpy, c->win, False, StructureNotifyMask, ( XEvent * ) &ce);
		if (status == 0) {
			dwmLog::FUNC(__func__);
			dwmLog::INFO("XSendEvent status=", status);
		}
	}
	static void focus(Client *c) {
		if (!c || ! ISVISIBLE(c)) {
			c = selmon->stack;
			while (c && ! ISVISIBLE(c)) { c = c->snext; }
		}
		if (c != selmon->sel) { unfocus(selmon->sel, 0); }
		if (c) {
			if (c->mon != selmon) { selmon = c->mon; }
			if (c->isurgent) { seturgent(c, 0); }
			detachstack(c);
			attachstack(c);
			grabbuttons(c, 1);
			setfocus(c);
		} else {
			XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
			XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
		}
		selmon->sel = c;
		drawbars();
	}
	static Atom getatomprop(Client *c, Atom prop) {
		int di;
		unsigned long dl;
		unsigned char *p = nullptr;
		Atom da, atom = None;

		if (XGetWindowProperty(dpy, c->win, prop, 0L, sizeof atom, False, XA_ATOM, &da, &di, &dl, &dl, &p) == Success && p) {
			atom = *( Atom * ) p;
			XFree(p);
		}
		return atom;
	}
	static void grabbuttons(Client *c, int focused) {
		updatenumlockmask();
		{
			unsigned int i, j;
			unsigned int modifiers[] = {0, LockMask, numlockmask, numlockmask | LockMask};

			XUngrabButton(dpy, AnyButton, AnyModifier, c->win);
			if (! focused) {
				XGrabButton(dpy, AnyButton, AnyModifier, c->win, False, BUTTONMASK, GrabModeSync, GrabModeSync, None, None);
			}
			for (i = 0; i < LENGTH(buttons); i++) {
				if (buttons[ i ].click == ClkClientWin) {
					for (j = 0; j < LENGTH(modifiers); j++) {
						XGrabButton(dpy, buttons[ i ].button, buttons[ i ].mask | modifiers[ j ], c->win, False, BUTTONMASK, GrabModeAsync, GrabModeSync, None, None);
					}
				}
			}
			XUngrabButton(dpy, AnyButton, AnyModifier, c->tB.win);
			if (! focused) {
				XGrabButton(dpy, AnyButton, AnyModifier, c->tB.win, False, BUTTONMASK, GrabModeSync, GrabModeSync, None, None);
			}
			for (i = 0; i < LENGTH(buttons); i++) {
				if (buttons[ i ].click == WmMove) {
					for (j = 0; j < LENGTH(modifiers); j++) {
						XGrabButton(dpy, buttons[ i ].button, buttons[ i ].mask | modifiers[ j ], c->tB.win, False, BUTTONMASK, GrabModeAsync, GrabModeSync, None, None);
					}
				}
			}
		}
	}
	static void unfocus(Client *c, int setfocus) {
		if (!c) { return; }
		grabbuttons(c, 0);
		if (setfocus) {
			XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
			XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
		}
	}
	static void unmanage(Client *c, int destroyed) {
		Monitor *m = c->mon;
		XWindowChanges wc;
		Client::detach(c);
		Client::detachstack(c);
		if (! destroyed) {
			/* avoid race conditions */
			XGrabServer(dpy);
			XSetErrorHandler(xerrordummy);
			XSelectInput(dpy, c->win, NoEventMask);
			/* restore border */
			XConfigureWindow(dpy, c->win, CWBorderWidth, &wc);
			XUngrabButton(dpy, AnyButton, AnyModifier, c->win);
			setclientstate(c, WithdrawnState);
			XSync(dpy, False);
			XSetErrorHandler(xerror);
			XUngrabServer(dpy);
		}
		free(c);
		Client::focus(nullptr);
		updateclientlist();
		arrange(m);
	}
	static void updatesizehints(Client *c) {
		long msize;
		XSizeHints size;

		/* Initializing all fields to zero */
		memset(&size, 0, sizeof(XSizeHints));

		if (! XGetWMNormalHints(dpy, c->win, &size, &msize))
			size.flags = PSize;
		bool hasBaseSize = size.flags & PBaseSize;
		bool hasMinSize = size.flags & PMinSize;
		bool hasMaxSize = size.flags & PMaxSize;
		bool hasResizeInc = size.flags & PResizeInc;
		// bool hasAspect = size.flags & PAspect;

		// Default values
		c->basew = c->baseh = c->incw = c->inch = c->maxw = c->maxh = c->minw = c->minh = 0;

		if (hasBaseSize) {
			c->basew = size.base_width;
			c->baseh = size.base_height;
		}
		else if (hasMinSize) {
			c->basew = size.min_width;
			c->baseh = size.min_height;
		}

		if (hasResizeInc) {
			c->incw = size.width_inc;
			c->inch = size.height_inc;
		}

		if (hasMaxSize) {
			c->maxw = size.max_width;
			c->maxh = size.max_height;
		}

		if (hasMinSize) {
			c->minw = size.min_width;
			c->minh = size.min_height;
		}
		else if (hasBaseSize) {
			c->minw = size.base_width;
			c->minh = size.base_height;
		}
		c->isfixed = (c->maxw && c->maxh && c->maxw == c->minw && c->maxh == c->minh);
		c->hintsvalid = 1;
	}
	static void updatetitle(Client *c) {
		if (! gettextprop(c->win, netatom[ NetWMName ], c->name, sizeof c->name)) {
			gettextprop(c->win, XA_WM_NAME, c->name, sizeof c->name);
		}
		/* hack to mark broken clients */
		if (c->name[ 0 ] == '\0') {
			strcpy(c->name, broken);
		}
	}
	static void updatewindowtype(Client *c) {
		Atom state = getatomprop(c, netatom[NetWMState]);
		Atom wtype = getatomprop(c, netatom[NetWMWindowType]);

		if (state == netatom[NetWMFullscreen]) { setfullscreen(c, 1); }
		if (wtype == netatom[NetWMWindowTypeDialog]) { c->isfloating = 1; }
	}
	static void updatewmhints(Client *c) {
		XWMHints *wmh;
		if ((wmh = XGetWMHints(dpy, c->win))) {
			if (c == selmon->sel && wmh->flags & XUrgencyHint) {
				wmh->flags &= ~XUrgencyHint;
				XSetWMHints(dpy, c->win, wmh);
			} else {
				c->isurgent = (wmh->flags & XUrgencyHint) ? 1 : 0;
			}
			if (wmh->flags & InputHint) {
				c->neverfocus = !wmh->input;
			} else {
				c->neverfocus = 0;
			}
			XFree(wmh);
		}
	}
	static void resize(Client *c, int x, int y, int w, int h, int interact) {
		if (applysizehints(c, &x, &y, &w, &h, interact)) { resizeclient(c, x, y, w, h); }
	}
	static void resizeclient(Client *c, int x, int y, int w, int h) {
		XWindowChanges wc;
		c->x = wc.x = x;
		c->y = wc.y = y;
		c->w = wc.width = w;
		c->h = wc.height = h;
		wc.border_width = 0;
		XConfigureWindow(dpy, c->win, CWX | CWY | CWWidth | CWHeight | CWBorderWidth, &wc);
		Client::configure(c);
		XSync(dpy, False);
	}
	static void setfocus(Client *c) {
		if (!c->neverfocus) {
			XSetInputFocus(dpy, c->win, RevertToPointerRoot, CurrentTime);
			XChangeProperty(dpy, root, netatom[ NetActiveWindow ], XA_WINDOW, 32, PropModeReplace, ( unsigned char * ) &(c->win), 1);
		}
		sendevent(c, wmatom[WMTakeFocus]);
	}
	static void setclientstate(Client *c, long state) {
		long data[] = {state, None};
		XChangeProperty(dpy, c->win, wmatom[ WMState ], wmatom[ WMState ], 32,
						PropModeReplace, ( unsigned char * ) data, 2);
	}
	static int sendevent(Client *c, Atom proto) {
		int n;
		Atom *protocols;
		int exists = 0;
		XEvent ev;

		if (XGetWMProtocols(dpy, c->win, &protocols, &n)) {
			while (!exists && n--) { exists = protocols[n] == proto; }
			XFree(protocols);
		}
		if (exists) {
			ev.type = ClientMessage;
			ev.xclient.window = c->win;
			ev.xclient.message_type = wmatom[ WMProtocols ];
			ev.xclient.format = 32;
			ev.xclient.data.l[ 0 ] = SafeConverter::UnsignedLongToLong(proto);
			ev.xclient.data.l[ 1 ] = CurrentTime;
			XSendEvent(dpy, c->win, False, NoEventMask, &ev);
		}
		return exists;
	}
	static void setfullscreen(Client *c, int fullscreen) {
		if (fullscreen && ! c->isfullscreen) {
			XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32, PropModeReplace, (unsigned char *)&netatom[NetWMFullscreen], 1);
			c->isfullscreen = 1;
			c->oldstate = c->isfloating;
			c->isfloating = 1;
			resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
			XRaiseWindow(dpy, c->win);
		}
		else if (! fullscreen && c->isfullscreen) {
			XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32, PropModeReplace, (unsigned char *)nullptr, 0);
			c->isfullscreen = 0;
			c->isfloating = c->oldstate;
			resizeclient(c, c->x, c->y, c->w, c->h);
			arrange(c->mon);
		}
	}
	static void seturgent(Client *c, int urg) {
		XWMHints *wmh;
		c->isurgent = urg;
		if (! (wmh = XGetWMHints(dpy, c->win))) { return; }
		wmh->flags = urg ? (wmh->flags | XUrgencyHint) : (wmh->flags & ~XUrgencyHint);
		XSetWMHints(dpy, c->win, wmh);
		XFree(wmh);
	}
};
class Event {
public:
	static std::unordered_map<int, void (*)(XEvent*)> handler;

	static void clientmessage(XEvent *e) {
		XClientMessageEvent *cme = &e->xclient;
		Client *c = wintoclient(cme->window);

		if (!c) { return; }
		if (cme->message_type == netatom[NetWMState]) {
			if (cme->data.l[ 1 ] == netatom[ NetWMFullscreen ] || cme->data.l[ 2 ] == netatom[ NetWMFullscreen ]) {
				Client::setfullscreen(c, (cme->data.l[0] == 1 /* _NET_WM_STATE_ADD    */ || (cme->data.l[0] == 2 /* _NET_WM_STATE_TOGGLE */ && !c->isfullscreen)));
			}
		} else if (cme->message_type == netatom[NetActiveWindow]) {
			if (c != selmon->sel && ! c->isurgent) {
				Client::seturgent(c, 1);
			}
		}
	}
	static void configurenotify(XEvent *e) {
		Monitor *m;
		Client *c;
		XConfigureEvent *ev = &e->xconfigure;
		int dirty;

		/* TODO: updategeom handling sucks, needs to be simplified */
		if (ev->window == root) {
			dirty = (sw != ev->width || sh != ev->height);
			sw = ev->width;
			sh = ev->height;
			if (updategeom() || dirty) {
				drw_resize(drw, sw, bh);
				updatebars();
				for (m = mons; m; m = m->next) {
					for (c = m->clients; c; c = c->next) {
						if (c->isfullscreen) {
							Client::resizeclient(c, m->mx, m->my, m->mw, m->mh);
						}
						XMoveResizeWindow(dpy, m->barwin, m->wx, m->by, m->ww, bh);
					}
				}
				Client::focus(nullptr);
				arrange(nullptr);
			}
		}
	}
	static void configurerequest(XEvent *e) {
		Client *c;
		Monitor *m;
		XConfigureRequestEvent *ev = &e->xconfigurerequest;
		XWindowChanges wc;

		if ((c = wintoclient(ev->window))) {
			if (c->isfloating || ! selmon->lt[ selmon->sellt ]->arrange) {
				m = c->mon;
				if (ev->value_mask & CWX) {
					c->x = m->mx + ev->x;
				}
				if (ev->value_mask & CWY) {
					c->y = m->my + ev->y;
				}
				if (ev->value_mask & CWWidth) {
					c->w = ev->width;
				}
				if (ev->value_mask & CWHeight) {
					c->h = ev->height;
				}
				if ((ev->value_mask & (CWX | CWY)) && ! (ev->value_mask & (CWWidth | CWHeight))) {
					Client::configure(c);
				}
				if (ISVISIBLE(c)) {
					XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
				}
			} else {
				Client::configure(c);
			}
		} else {
			wc.x = ev->x;
			wc.y = ev->y;
			wc.width = ev->width;
			wc.height = ev->height;
			wc.border_width = ev->border_width;
			wc.sibling = ev->above;
			wc.stack_mode = ev->detail;
			XConfigureWindow(dpy, ev->window, ev->value_mask, &wc);
		}
		XSync(dpy, False);
	}
	static void destroynotify(XEvent *e) {
		Client *c;
		XDestroyWindowEvent *ev = &e->xdestroywindow;

		if ((c = wintoclient(ev->window))) {
			Client::unmanage(c, 1);
		}
	}
	static void enternotify(XEvent *e) {
		Client *c;
		Monitor *m;
		XCrossingEvent *ev = &e->xcrossing;

		if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != root) {
			return;
		}
		c = wintoclient(ev->window);
		m = c ? c->mon : wintomon(ev->window);

		if (m != selmon) {
			Client::unfocus(selmon->sel, 1);
			selmon = m;
		} else if (! c || c == selmon->sel) {
			return;
		}
		Client::focus(c);
	}
	static void expose(XEvent *e) {
		XExposeEvent *ev = &e->xexpose;
		Monitor *m = wintomon(ev->window);
		if (m && ev->count == 0) {
			drawbar(m);
		}
	}
	/* there are some broken focus acquiring
     * clients needing extra handling 		 */
	static void focusin(XEvent *e) {
		XFocusChangeEvent *ev = &e->xfocus;
		if (selmon->sel && ev->window != selmon->sel->win) {
			Client::setfocus(selmon->sel);
		}
	}
	static void keypress(XEvent *e) {
		dwmLog::FUNC(__func__);

		unsigned int i;
		KeySym keysym;
		XKeyEvent *ev;
		ev = &e->xkey;

		/* Updated function call */
		keysym = XkbKeycodeToKeysym(dpy, ( KeyCode ) ev->keycode, 0, 0);
		for (i = 0; i < LENGTH(keys); i++) {
			if (keysym == keys[ i ].keysym && CLEANMASK(keys[ i ].mod) == CLEANMASK(ev->state) && keys[ i ].func) {
				keys[ i ].func(&(keys[ i ].arg));
			}
		}

		dwmLog::ENDFUNC(__func__);
	}
	static void mappingnotify(XEvent *e) {
		XMappingEvent *ev = &e->xmapping;
		XRefreshKeyboardMapping(ev);
		if (ev->request == MappingKeyboard) {
			grabkeys();
		}
	}
	static void motionnotify(XEvent *e) {
		static Monitor *mon = nullptr;
		Monitor *m;
		XMotionEvent *ev = &e->xmotion;

		if (ev->window != root) {
			return;
		}
		if ((m = recttomon(ev->x_root, ev->y_root, 1, 1)) != mon && mon) {
			Client::unfocus(selmon->sel, 1);
			selmon = m;
			Client::focus(nullptr);
		}
		mon = m;
	}
	static void propertynotify(XEvent *e) {
		Client *c;
		Window trans;
		XPropertyEvent *ev = &e->xproperty;

		if ((ev->window == root) && (ev->atom == XA_WM_NAME)) {
			updatestatus();
		}
		else if (ev->state == PropertyDelete) {
			return; /* ignore */
		}
		else if ((c = wintoclient(ev->window))) {
			switch (ev->atom) {
				default:
					break;
				case XA_WM_TRANSIENT_FOR:
					if (! c->isfloating && (XGetTransientForHint(dpy, c->win, &trans)) &&
						(c->isfloating = (wintoclient(trans)) != nullptr))
						arrange(c->mon);
					break;
				case XA_WM_NORMAL_HINTS:
					c->hintsvalid = 0;
					break;
				case XA_WM_HINTS:
					Client::updatewmhints(c);
					drawbars();
					break;
			}
			if (ev->atom == XA_WM_NAME || ev->atom == netatom[ NetWMName ]) {
				Client::updatetitle(c);
				if (c == c->mon->sel) {
					drawbar(c->mon);
				}
			}
			if (ev->atom == netatom[ NetWMWindowType ]) {
				Client::updatewindowtype(c);
			}
		}
	}
	static void unmapnotify(XEvent *e) {
		Client *c;
		XUnmapEvent *ev = &e->xunmap;

		if ((c = wintoclient(ev->window))) {
			if (ev->send_event) {
				Client::setclientstate(c, WithdrawnState);
			}
			else {
				Client::unmanage(c, 0);
			}
		}
	}
};
std::unordered_map<int, void (*)(XEvent*)> Event::handler = {
	{ButtonPressMask, 	buttonpress		},
	{ClientMessage, 	clientmessage	},
	{ConfigureRequest, 	configurerequest},
	{ConfigureNotify, 	configurenotify	},
	{DestroyNotify, 	destroynotify	},
	{EnterNotify, 		enternotify		},
	{Expose, 			expose			},
	{FocusIn, 			focusin			},
	{KeyPress, 			keypress		},
	{MappingNotify, 	mappingnotify	},
	{MapRequest, 		maprequest		},
	{MotionNotify, 		motionnotify	},
	{PropertyNotify, 	propertynotify	},
	{UnmapNotify, 		unmapnotify		}
};
/* function implementations */
void applyrules ( Client *c ) {
	const char *class_name, *instance;
	unsigned int i;
	const Rule *r;
	Monitor *m;
	XClassHint ch = { nullptr, nullptr };

	/* rule matching */
	c->isfloating = 1;
	c->tags = 0;
	XGetClassHint(dpy, c->win, &ch);
	class_name = ch.res_class ? ch.res_class : broken;
	instance = ch.res_name ? ch.res_name : broken;

	for ( i = 0; i < LENGTH ( rules ); i++ ) {
		r = &rules[ i ];

		if ((!r->title || strstr(c->name, r->title)) &&
			(!r->class_name || strstr(class_name, r->class_name)) &&
			(!r->instance || strstr(instance, r->instance))) {
			c->isfloating = r->isfloating;
			c->tags |= r->tags;

			for (m = mons; m && m->num != r->monitor; m = m->next) {}
			if (m) { c->mon = m; }
		}
	}
	if ( ch.res_class ) { XFree(ch.res_class); }
	if ( ch.res_name ) { XFree(ch.res_name); }
	c->tags = c->tags & TAGMASK ? c->tags & TAGMASK : c->mon->tagset[ c->mon->seltags ];
}
int applysizehints(Client *c, const int *x, const int *y, int *w, int *h, int interact) {
	int baseismin;

	/* set minimum possible */
	*w = MAX(1, *w);
	*h = MAX(1, *h);

	if (*h < bh) {
		*h = bh;
	}
	if (*w < bh) {
		*w = bh;
	}

	if (resizehints || c->isfloating || ! c->mon->lt[ c->mon->sellt ]->arrange) {
		if (!c->hintsvalid) { Client::updatesizehints(c); }
		/* see last two sentences in ICCCM 4.1.2.3 */
		baseismin = c->basew == c->minw && c->baseh == c->minh;
		/* temporarily remove base dimensions */
		if ( ! baseismin ) {
			*w -= c->basew;
			*h -= c->baseh;
		}
		/* increment calculation requires this */
		if (baseismin) {
			*w -= c->basew;
			*h -= c->baseh;
		}
		/* adjust for increment value */
		if (c->incw) {
			*w -= *w % c->incw;
		}
		if (c->inch) {
			*h -= *h % c->inch;
		}
		/* restore base dimensions */
		*w = MAX (*w + c->basew, c->minw);
		*h = MAX (*h + c->baseh, c->minh);
		if (c->maxw) {
			*w = MIN(*w, c->maxw);
		}
		if (c->maxh) {
			*h = MIN(*h, c->maxh);
		}
	}
	return *x != c->x || *y != c->y || *w != c->w || *h != c->h;
}
void arrange ( Monitor *m ) {
	Monitor *monitor = m ? m : mons;
	while (monitor) {
		if (m) {
			arrangemon(monitor);
			restack(monitor);
			break;
		}
		monitor = monitor->next;
	}
}
void arrangemon ( Monitor *m ) {
	strncpy ( m->ltsymbol, m->lt[ m->sellt ]->symbol, sizeof m->ltsymbol );
	if ( m->lt[ m->sellt ]->arrange )
		m->lt[ m->sellt ]->arrange ( m );
}
#ifndef BUTTONPRESS
#define BUTTONPRESS
void focusMonitor(XButtonPressedEvent *ev) {
    Monitor *m;
    if ((m = wintomon(ev->window)) && m != selmon) {
        Client::unfocus(selmon->sel, 1);
        selmon = m;
        Client::focus(nullptr);
    }
}
unsigned int processBarClick(XButtonPressedEvent *ev) {
    Arg arg = { 0 };
    unsigned int i, x, click = ClkRootWin;
    i = x = 0;

	do (x += TEXTW(tags[i]));
    while (ev->x >= x && ++i < LENGTH(tags));

    if (i < LENGTH(tags)) {
        click = ClkTagBar;
        arg.ui = 1 << i;
    } else if (ev->x < x + TEXTW(selmon->ltsymbol)) {
        click = ClkLtSymbol;
	} else if (ev->x > selmon->ww - (int)TEXTW(stext)) {
		click = ClkStatusText;
	} else {
        click = ClkWinTitle;
	}
    return click;
}
unsigned int handleClientWindowClick(XButtonPressedEvent *ev) {
    Client *c;
    unsigned int click = ClkRootWin;

    if (( c = wintoclient( ev->window ))) {
        Client::focus(c);
        restack( selmon );
        XAllowEvents( dpy, ReplayPointer, CurrentTime );
        click = ClkClientWin;
    }
    return click;
}
void executeButtonFunction(unsigned int click, XButtonPressedEvent *ev) {
    Arg arg = {0};
	for (const auto & button : buttons) {
		if (click == button.click && button.func && button.button == ev->button && CLEANMASK(button.mask) == CLEANMASK(ev->state)) {
			button.func( click == ClkTagBar && button.arg.i == 0 ? &arg : &button.arg );
		}
	}
}
void buttonpress(XEvent *e) {
    XButtonPressedEvent *ev = &e->xbutton;
    unsigned int click = ClkRootWin;

    focusMonitor(ev); /* Focus monitor if necessary */
    /* Handle bar and client window clicks */
    if (ev->window == selmon->barwin) {
        click = processBarClick(ev);
	} else if (ev->window == selmon->sel->tB.win) { /* If titleBar win is clicked move the window */
		movemouse(nullptr);
	} else {
        click = handleClientWindowClick(ev);
	}
	if (ev->window == selmon->sel->win) {
		Client::focus(selmon->sel);
	}
    /* Execute the corresponding button function */
    executeButtonFunction(click, ev);
}
#endif/*BUTTONPRESS*/
void checkotherwm() { /* this causes an error if some other window manager is running */
	xerrorxlib = XSetErrorHandler(xerrorstart);
	XSelectInput(dpy, DefaultRootWindow(dpy), SubstructureRedirectMask);
	XSync(dpy, False);
	XSetErrorHandler(xerror);
	XSync(dpy, False);
}
void cleanup() {
	Arg a = {.ui = static_cast<unsigned int>(~0)};
	Layout foo = {"", nullptr};
	Monitor *m;
	size_t i;
	view(&a);
	selmon->lt[ selmon->sellt ] = &foo;

	for (m = mons; m; m = m->next) {
		while (m->stack) {
			Client::unmanage(m->stack, 0);
		}
	}
	XUngrabKey(dpy, AnyKey, AnyModifier, root);
	while (mons) {
		cleanupmon(mons);
	}
	for (i = 0; i < CurLast; i++) {
		drw_cur_free(drw, cursor[ i ]);
	}
	for (i = 0; i < LENGTH(colors); i++) {
		free(scheme[ i ]);
	}
	free(scheme);
	XDestroyWindow(dpy, wmcheckwin);
	drw_free(drw);
	XSync(dpy, False);
	XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
	XDeleteProperty(dpy, root, netatom[ NetActiveWindow ]);
}
void cleanupmon(Monitor *mon) {
	Monitor *m;
	if (mon == mons) { mons = mons->next; }
	else {
		for (m = mons; m && m->next != mon; m = m->next) {}
		m->next = mon->next;
	}
	XUnmapWindow(dpy, mon->barwin);
	XDestroyWindow(dpy, mon->barwin);
	free(mon);
}
Monitor *createmon() {
	std::unique_ptr<Monitor> m = std::make_unique<Monitor>();
	m->tagset[ 0 ] = m->tagset[ 1 ] = 1;
	m->showbar = showbar;
	m->topbar = topbar;
	m->lt[0] = &layouts[0];
	m->lt[1] = &layouts[1 % LENGTH(layouts)];
	strncpy(m->ltsymbol, layouts[0].symbol, sizeof(m->ltsymbol));
	return m.release();// Release ownership and return the raw pointer
}
void drawbar(Monitor *m) {
	int x, w, tw = 0;
	int boxs = SafeConverter::UnsignedToInt(drw->fonts->h) / 9;
	int boxw = SafeConverter::UnsignedToInt(drw->fonts->h) / 6 + 2;
	unsigned int i, occ = 0, urg = 0;
	Client *c;

	if (! m->showbar) {
		return;
	}
	/* draw status first, so it can be overdrawn by tags later
   * status is only drawn on selected monitor */
	if (m == selmon) {
		drw_setscheme(drw, scheme[ SchemeNorm ]);
		tw = TEXTW(stext) - lrpad + 2; /* 2px right padding */
		drw_text(drw, m->ww - tw, 0, tw, bh, 0, stext, 0);
	}
	for (c = m->clients; c; c = c->next) {
		occ |= c->tags;
		if (c->isurgent) {
			urg |= c->tags;
		}
	}
	x = 0;
	for (i = 0; i < LENGTH(tags); i++) {
		w = TEXTW(tags[ i ]);
		drw_setscheme(drw, scheme[ m->tagset[ m->seltags ] & 1 << i ? SchemeSel : SchemeNorm ]);
		drw_text(drw, x, 0, w, bh, lrpad / 2, tags[ i ], SafeConverter::UnsignedToInt(urg) & 1 << i);
		if (occ & 1 << i) {
			drw_rect(drw, x + boxs, boxs, boxw, boxw, m == selmon && selmon->sel && selmon->sel->tags & 1 << i, SafeConverter::UnsignedToInt(urg) & 1 << i);
		}
		x += w;
	}
	w = TEXTW(m->ltsymbol);
	drw_setscheme(drw, scheme[ SchemeNorm ]);
	x = drw_text(drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0);

	if ((w = m->ww - tw - x) > bh) {
		if (m->sel) {
			drw_setscheme(drw, scheme[ m == selmon ? SchemeSel : SchemeNorm ]);
			drw_text(drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0);
			if (m->sel->isfloating) {
				drw_rect(drw, x + boxs, boxs, boxw, boxw, m->sel->isfixed, 0);
			}
		}
		else {
			drw_setscheme(drw, scheme[ SchemeNorm ]);
			drw_rect(drw, x, 0, w, bh, 1, 1);
		}
	}
	drw_map(drw, m->barwin, 0, 0, m->ww, bh);
}
void drawbars() {
	Monitor *m;
	for (m = mons; m; m = m->next) {
		drawbar(m);
	}
}
void focusstack(const Arg *arg) {
	if (! selmon->sel || (selmon->sel->isfullscreen & lockfullscreen)) {
		return;
	}
	Client *next_client = nullptr;
	if (arg->i > 0) {
		for (next_client = selmon->sel->next ? selmon->sel->next : selmon->clients; next_client && ! ISVISIBLE(next_client); next_client = next_client->next ? next_client->next : selmon->clients) {}
	}
	else { /* Find the previous visible client before the currently selected client. */
		for (Client *i = selmon->clients; i; i = i->next) {
			if (ISVISIBLE(i)) {
				next_client = i;
				if (i == selmon->sel) {
					break;
				}
			}
		}
	}
	if (next_client) {
		Client::focus(next_client);
		restack(selmon);
	}
}
int getrootptr(int *x, int *y) {
	int di;
	unsigned int dui;
	Window dummy;
	return XQueryPointer(dpy, root, &dummy, &dummy, x, y, &di, &di, &dui);
}
long getstate(Window w) {
	int format;
	long result = -1;
	unsigned char *p = nullptr;
	unsigned long n, extra;
	Atom real;

	if (XGetWindowProperty(dpy, w, wmatom[ WMState ], 0L, 2L, False, wmatom[ WMState ], &real, &format, &n, &extra, ( unsigned char ** ) &p) != Success) {
		return -1;
	}
	if (n != 0) {
		result = *p;
	}
	XFree(p);
	return result;
}
int gettextprop(Window w, Atom atom, char *text, unsigned int size) {
	char **list = nullptr;
	int n;
	XTextProperty name;

	if (! text || size == 0) {
		return 0;
	}
	text[ 0 ] = '\0';
	if (! XGetTextProperty(dpy, w, &name, atom) || ! name.nitems) {
		return 0;
	}
	if (name.encoding == XA_STRING) {
		strncpy(text, ( char * ) name.value, size - 1);
	}
	else if (XmbTextPropertyToTextList(dpy, &name, &list, &n) >= Success && n > 0 && *list) {
		strncpy(text, *list, size - 1);
		XFreeStringList(list);
	}
	text[ size - 1 ] = '\0';
	XFree(name.value);
	return 1;
}
void grabkeys() {
	dwmLog::FUNC(__func__);

	updatenumlockmask();
	{
		unsigned int i, j, k;
		unsigned int modifiers[] = {0, LockMask, numlockmask, numlockmask | LockMask};
		int start, end, skip;
		KeySym *syms;

		XUngrabKey(dpy, AnyKey, AnyModifier, root);
		XDisplayKeycodes(dpy, &start, &end);
		syms = XGetKeyboardMapping(dpy, start, end - start + 1, &skip);
		if (! syms) {
			return;
		}
		for (k = start; k <= end; k++) {
			for (i = 0; i < LENGTH(keys); i++) {
				if (keys[ i ].keysym == syms[ (k - start) * skip ]) {
					for (j = 0; j < LENGTH(modifiers); j++) {
						XGrabKey(dpy, SafeConverter::UnsignedToInt(k), keys[ i ].mod | modifiers[ j ], root, True, GrabModeAsync, GrabModeAsync);
					}
				}
			}
		}
		XFree(syms);
	}

	dwmLog::ENDFUNC(__func__);
}
void killclient(const Arg *arg) {
	dwmLog::FUNC(__func__);

	if (! selmon->sel) {
		return;
	}
	if (! Client::sendevent(selmon->sel, wmatom[ WMDelete ])) {
		XGrabServer(dpy);
		XSetErrorHandler(xerrordummy);
		XSetCloseDownMode(dpy, DestroyAll);
		XKillClient(dpy, selmon->sel->win);
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		XUngrabServer(dpy);
	}

	dwmLog::ENDFUNC(__func__);
}
class Manage {
public:
	/*
		??
		XSelectInput(dpy, w, EnterWindowMask | FocusChangeMask | PropertyChangeMask | StructureNotifyMask);
		Client::grabbuttons(c, 0);

		// some windows require this
		XChangeProperty(dpy, root, netatom[ NetClientList ], XA_WINDOW, 32, PropModeAppend, ( unsigned char * ) &(c->win), 1);
		XMoveResizeWindow(dpy, c->win, c->x + 2 * sw, c->y, c->w, c->h);
		Client::setclientstate(c, NormalState);
		XMoveResizeWindow(dpy, c->win, c->x + 2 * sw, c->y, c->w, c->h);
		Client::setclientstate(c, NormalState);

		// maybe IMPORTENT
		if (c->mon == selmon) {
			Client::unfocus(selmon->sel, 0);
		}

	*/
	static void win(Window w, XWindowAttributes *wa){
		Client *c, *t = nullptr;
		Window trans = None;
		c = static_cast<Client *>(ecalloc(1, sizeof(Client)));

		requestSize(c, wa, w);
		Client::updatetitle(c);
		XSelectInput(dpy, w, EnterWindowMask | FocusChangeMask | PropertyChangeMask | StructureNotifyMask);
		applyWinDecorations(c);
		getTrans(c, t, w, trans);
		/* 	Floating seems to make the windows
		*	own move areas interactebe 			*/
		checkFloating(c, trans);
		/* IMPORTENT */
		Client::attach(c);
		/* IMPORTENT */
		Client::attachstack(c);
		/* THIS seams to actually
		 * focus the window and raise it */
		c->mon->sel = c;
		XMapWindow(dpy, c->win);

		/* Without this movemouse and resizemouce
         * CANNOT interact with window 				*/
		Client::focus(nullptr);
	};
private:
	static void getTrans(Client *c, Client *t, Window w, Window trans) {
		/* IMPORTENT for fullscreen windows like games when steam overlany dissepears */
		if (XGetTransientForHint(dpy, w, &trans) && (t = wintoclient(trans))) {
			c->mon = t->mon;
			c->tags = t->tags;
		} else {
			c->mon = selmon;
			applyrules(c);
		}
	}
	static void requestSize(Client *c, XWindowAttributes *wa, Window w) {
		c->win = w;

		/* geometry */
		c->x = wa->x;
		c->y = wa->y;
		c->w = wa->width;
		c->h = wa->height;
	}
	static void checkFloating(Client *c, Window trans) {
		if (! c->isfloating) {
			if (trans == None) {
				c->isfloating = c->oldstate = ! ! c->isfixed;
			}
			else {
				c->isfloating = c->oldstate = true;
			}
		}
		if (c->isfloating) {
			XRaiseWindow(dpy, c->win);
		}
	}
	/*
	void checkAndMakeWinFit(Client *c) {
		// Check if window X-AXIS is grater then monitors X-AXI
		if (c->x + WIDTH(c) > c->mon->wx + c->mon->ww) {
			// If TRUE then Make Window X equal to monitor X
			c->x = c->mon->wx + c->mon->ww - WIDTH(c);
		}
		// Check if window Y-AXIS is grater then monitors Y-AXI
		if (c->y + HEIGHT(c) > c->mon->wy + c->mon->wh) {
			// If TRUE then Make Window Y equal to monitor Y
			c->y = c->mon->wy + c->mon->wh - HEIGHT(c);
		}
		c->x = MAX(c->x, c->mon->wx); // ? //
		c->y = MAX(c->y, c->mon->wy); // ? //
	}
	*/
};
void maprequest(XEvent *e) {
	static XWindowAttributes wa;
	XMapRequestEvent *ev = &e->xmaprequest;
	if (!XGetWindowAttributes(dpy, ev->window, &wa) || wa.override_redirect) { return; }
	if (!wintoclient(ev->window)) { Manage::win(ev->window, &wa); }
}
void movemouse(const Arg *arg) {
	int x, y, ocx, ocy, nx, ny;
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if (!(c = selmon->sel)) { return; }
	/* no support moving fullscreen windows by mouse */
	if (c->isfullscreen) { return; }
	restack(selmon);
	ocx = c->x;
	ocy = c->y;
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync, None, cursor[CurMove]->cursor, CurrentTime) != GrabSuccess) { return; }
	if (!getrootptr(&x, &y)) { return; }
	do {
		XMaskEvent(dpy, MOUSEMASK | ExposureMask | SubstructureRedirectMask, &ev);
		switch (ev.type) {
			case ConfigureRequest:
			case Expose:
			case MapRequest:
				Event::handler[ ev.type ](&ev);
				break;
			case MotionNotify:
				if ((ev.xmotion.time - lasttime) <= (1000 / 240)) { continue; }
				lasttime = ev.xmotion.time;
				nx = ocx + (ev.xmotion.x - x);
				ny = ocy + (ev.xmotion.y - y);
				if (std::abs(selmon->wx - nx) < snap) {
					nx = selmon->wx;
				} else if (std::abs((selmon->wx + selmon->ww) - (nx + WIDTH(c))) < snap) {
					nx = selmon->wx + selmon->ww - WIDTH(c);
				}
				if (std::abs(selmon->wy - ny) < snap) {
					ny = selmon->wy;
				} else if (std::abs((selmon->wy + selmon->wh) - (ny + HEIGHT(c))) < snap) {
					ny = selmon->wy + selmon->wh - HEIGHT(c);
				}
				if (! c->isfloating && selmon->lt[ selmon->sellt ]->arrange && (std::abs(nx - c->x) > snap || std::abs(ny - c->y) > snap)) { togglefloating(nullptr); }
				if (! selmon->lt[ selmon->sellt ]->arrange || c->isfloating) { Client::resize(c, nx, ny, c->w, c->h, 1); }
				break;
		}
	} while (ev.type != ButtonRelease);
	XUngrabPointer(dpy, CurrentTime);
	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		sendmon(c, m);
		selmon = m;
		Client::focus(nullptr);
	}
}
Monitor *recttomon(int x, int y, int w, int h) {
	Monitor *m, *r = selmon;
	int a, area = 0;
	for (m = mons; m; m = m->next) {
		if ((a = INTERSECT(x, y, w, h, m)) > area) {
			area = a;
			r = m;
		}
	}
	return r;
}
void resizemouse(const Arg *arg) {
	int ocx, ocy, nw, nh;
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if (! (c = selmon->sel)) {
		return;
	}
	if (c->isfullscreen) {
		return; /* no support resizing fullscreen windows by mouse */
	}
	restack(selmon);
	ocx = c->x;
	ocy = c->y;
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync, None, cursor[ CurResize ]->cursor, CurrentTime) != GrabSuccess) {
		return;
	}
	/* Moves the mouse to the right bottom corner before resizeing so that it has a refrance frame */
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w - 1, c->h - 1);
	do {
		XMaskEvent(dpy, MOUSEMASK | ExposureMask | SubstructureRedirectMask, &ev);
		switch (ev.type) {
			case ConfigureRequest:
			case Expose:
			case MapRequest:
				Event::handler[ ev.type ](&ev);
				break;
			case MotionNotify:
				if ((ev.xmotion.time - lasttime) <= (1000 / 240)) {
					continue;
				}
				lasttime = ev.xmotion.time;
				nw = MAX(ev.xmotion.x - ocx - 2, 1);
				nh = MAX(ev.xmotion.y - ocy - 2, 1);
				if (c->mon->wx + nw >= selmon->wx &&
					c->mon->wx + nw <= selmon->wx + selmon->ww &&
					c->mon->wy + nh >= selmon->wy &&
					c->mon->wy + nh <= selmon->wy + selmon->wh) {
					if (! c->isfloating && selmon->lt[ selmon->sellt ]->arrange &&
						(std::abs(nw - c->w) > snap || std::abs(nh - c->h) > snap)) {
						togglefloating(nullptr);
					}
				}
				if (! selmon->lt[ selmon->sellt ]->arrange || c->isfloating) {
					Client::resize(c, c->x, c->y, nw, nh, 1);
				}
				break;
		}
	} while (ev.type != ButtonRelease);
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w - 1, c->h - 1);
	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev)) {}

	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		sendmon(c, m);
		selmon = m;
		Client::focus(nullptr);
	}
}
void restack(Monitor *m) {
	Client *c;
	XEvent ev;
	XWindowChanges wc;
	drawbar(m);

	if (!m->sel) { return; }
	if (m->sel->isfloating || ! m->lt[ m->sellt ]->arrange) { XRaiseWindow(dpy, m->sel->win); }
	if (m->lt[ m->sellt ]->arrange) {
		wc.stack_mode = Below;
		wc.sibling = m->barwin;

		for (c = m->stack; c; c = c->snext) {
			if (! c->isfloating && ISVISIBLE(c)) {
				XConfigureWindow(dpy, c->win, CWSibling | CWStackMode, &wc);
				wc.sibling = c->win;
			}
		}
	}
	XSync(dpy, False);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev)) {}
}
void run() {
	XEvent ev;
	while (running) {
		XNextEvent(dpy, &ev);
		if (Event::handler[ev.type]) { Event::handler[ev.type](&ev); }
	}
}
void scan() {
	unsigned int i, num;
	Window d1, d2, *wins = nullptr;
	XWindowAttributes wa;

	if (XQueryTree(dpy, root, &d1, &d2, &wins, &num)) {
		for (i = 0; i < num; i++) {
			if (! XGetWindowAttributes(dpy, wins[ i ], &wa) || wa.override_redirect || XGetTransientForHint(dpy, wins[ i ], &d1)) { continue; }
			if (wa.map_state == IsViewable || getstate(wins[ i ]) == IconicState) { Manage::win(wins[ i ], &wa); }
		}
		for (i = 0; i < num; i++) { /* now the transients */
			if (! XGetWindowAttributes(dpy, wins[ i ], &wa)) { continue; }
			if (XGetTransientForHint(dpy, wins[ i ], &d1) && (wa.map_state == IsViewable || getstate(wins[ i ]) == IconicState)) { Manage::win(wins[ i ], &wa); }
		}
		if (wins) { XFree(wins); }
	}
}
void sendmon(Client *c, Monitor *m) {
	if (c->mon == m) { return; }
	Client::unfocus(c, 1);
	Client::detach(c);
	Client::detachstack(c);
	c->mon = m;
	c->tags = m->tagset[m->seltags]; /* assign tags of target monitor */
	Client::attach(c);
	Client::attachstack(c);
	Client::focus(nullptr);
	arrange(nullptr);
}
void setup() {
	int i;
	XSetWindowAttributes wa;
	Atom utf8string;
	struct sigaction sa {};
	/* do not transform children into zombies when they terminate */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT | SA_RESTART;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGCHLD, &sa, nullptr);
	/* clean up any zombies (inherited from .xinitrc etc.) immediately */
	while (waitpid(-1, nullptr, WNOHANG) > 0) {}
	/* init screen */
	screen = DefaultScreen(dpy);
	sw = DisplayWidth(dpy, screen);
	sh = DisplayHeight(dpy, screen);
	root = RootWindow(dpy, screen);
	drw = drw_create(dpy, screen, root, sw, sh);
	if (!drw_fontset_create(drw, fonts, LENGTH(fonts))) { die("no fonts could be loaded."); }
	lrpad = SafeConverter::UnsignedToInt(drw->fonts->h);
	bh = SafeConverter::UnsignedToInt(drw->fonts->h) + 2;
	updategeom();
	/* init atoms */
	utf8string = XInternAtom(dpy, "UTF8_STRING", False);
	wmatom[WMProtocols] = XInternAtom(dpy, "WM_PROTOCOLS", False);
	wmatom[WMDelete] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	wmatom[WMState] = XInternAtom(dpy, "WM_STATE", False);
	wmatom[WMTakeFocus] = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
	netatom[NetActiveWindow] = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
	netatom[NetSupported] = XInternAtom(dpy, "_NET_SUPPORTED", False);
	netatom[NetWMName] = XInternAtom(dpy, "_NET_WM_NAME", False);
	netatom[NetWMState] = XInternAtom(dpy, "_NET_WM_STATE", False);
	netatom[NetWMCheck] = XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", False);
	netatom[NetWMFullscreen] =
		XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
	netatom[NetWMWindowType] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
	netatom[NetWMWindowTypeDialog] =
		XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
	netatom[NetClientList] = XInternAtom(dpy, "_NET_CLIENT_LIST", False);
	/* init cursors */
	cursor[CurNormal] = drw_cur_create(drw, XC_left_ptr);
	cursor[CurResize] = drw_cur_create(drw, XC_sizing);
	cursor[CurMove] = drw_cur_create(drw, XC_fleur);
	/* init appearance */
	scheme = static_cast<Clr **>(ecalloc(LENGTH(colors), sizeof(Clr *)));
	for (i = 0; i < LENGTH(colors); i++) {
		const char *clrnames[] = {colors[i].fg, colors[i].bg, colors[i].border};
		scheme[i] = drw_scm_create(drw, clrnames, 3);// Pass 3 because there are 3 colors in each scheme
	}
	/* init bars */
	updatebars();
	updatestatus();
	/* supporting window for NetWMCheck */
	wmcheckwin = XCreateSimpleWindow(dpy, root, 0, 0, 1, 1, 0, 0, 0);
	XChangeProperty(dpy, wmcheckwin, netatom[ NetWMCheck ], XA_WINDOW, 32, PropModeReplace, ( unsigned char * ) &wmcheckwin, 1);
	XChangeProperty(dpy, wmcheckwin, netatom[ NetWMName ], utf8string, 8, PropModeReplace, ( unsigned char * ) "dwm", 3);
	XChangeProperty(dpy, root, netatom[ NetWMCheck ], XA_WINDOW, 32, PropModeReplace, ( unsigned char * ) &wmcheckwin, 1);
	/* EWMH support per view */
	XChangeProperty(dpy, root, netatom[ NetSupported ], XA_ATOM, 32, PropModeReplace, ( unsigned char * ) netatom, NetLast);
	XDeleteProperty(dpy, root, netatom[ NetClientList ]);
	/* select events */
	wa.cursor = cursor[ CurNormal ]->cursor;
	wa.event_mask = SubstructureRedirectMask | SubstructureNotifyMask |
					ButtonPressMask | PointerMotionMask | EnterWindowMask |
					LeaveWindowMask | StructureNotifyMask | PropertyChangeMask;
	XChangeWindowAttributes(dpy, root, CWEventMask | CWCursor, &wa);
	XSelectInput(dpy, root, wa.event_mask);
	grabkeys();
	Client::focus(nullptr);
}
void spawn(const Arg *arg) {
	struct sigaction sa {};
	if (fork() == 0) {
		if (dpy) { close(ConnectionNumber(dpy)); }
		setsid();
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sa.sa_handler = SIG_DFL;
		sigaction(SIGCHLD, &sa, nullptr);
		execvp(((char **)arg->v)[0], (char **)arg->v);
		die("shit FAILED: end !!");
	}
}
void togglebar(const Arg *arg) {
	selmon->showbar = ! selmon->showbar;
	updatebarpos(selmon);
	XMoveResizeWindow(dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww, bh);
	arrange(selmon);
}
void togglefloating(const Arg *arg) {
	if (!selmon->sel) { return; }
	/* no support for fullscreen windows */
	if (selmon->sel->isfullscreen) { return; }
	selmon->sel->isfloating = ! selmon->sel->isfloating || selmon->sel->isfixed;
	if (selmon->sel->isfloating) {
		Client::resize(selmon->sel, selmon->sel->x, selmon->sel->y, selmon->sel->w, selmon->sel->h, 0);
	}
	arrange(selmon);
}
void updatebars() {
	Monitor *m;
	XSetWindowAttributes wa;
	wa.override_redirect = True;
	wa.background_pixmap = ParentRelative;
	wa.event_mask = ButtonPressMask | ExposureMask;
	char className[] = "dwm";
	char classTitle[] = "dwm";
	XClassHint ch = {className, classTitle};

	for (m = mons; m; m = m->next) {
		if (m->barwin) { continue; }
		m->barwin = XCreateWindow(	dpy, root, m->wx, m->by, m->ww, bh, 0, DefaultDepth(dpy, screen),
									CopyFromParent, DefaultVisual(dpy, screen),
									CWOverrideRedirect | CWBackPixmap | CWEventMask, &wa);
		XDefineCursor(dpy, m->barwin, cursor[CurNormal]->cursor);
		XMapRaised(dpy, m->barwin);
		XSetClassHint(dpy, m->barwin, &ch);
	}
}
void updatebarpos(Monitor *m) {
	m->wy = m->my;
	m->wh = m->mh;

	if (m->showbar) {
		m->wh -= bh;
		m->by = m->topbar ? m->wy : m->wy + m->wh;
		m->wy = m->topbar ? m->wy + bh : m->wy;
	}
	else { m->by = -bh; }
}
void updateclientlist() {
	Client *c;
	Monitor *m;
	XDeleteProperty(dpy, root, netatom[NetClientList]);

	for (m = mons; m; m = m->next) {
		for (c = m->clients; c; c = c->next) {
			XChangeProperty(dpy, root, netatom[ NetClientList ], XA_WINDOW, 32, PropModeAppend, ( unsigned char * ) &(c->win), 1);
		}
	}

	dwmLog::ENDFUNC(__func__);
}
int updategeom() {
	int dirty = 0;
	if (!mons) { mons = createmon(); }
	if (mons->mw != sw || mons->mh != sh) { /* Check if screen size has changed */
		dirty = 1;
		mons->mw = mons->ww = sw;
		mons->mh = mons->wh = sh;
		updatebarpos(mons);
	}
	/* Directly set selmon to the result of wintomon */
	if (dirty) { selmon = wintomon(root); }
	return dirty;
}
void updatenumlockmask() {
	unsigned int i, j;
	numlockmask = 0;
	XModifierKeymap *modmap;
	modmap = XGetModifierMapping(dpy);

	for (i = 0; i < 8; i++) {
		for (j = 0; j < modmap->max_keypermod; j++) {
			if (modmap->modifiermap[ i * modmap->max_keypermod + j ] == XKeysymToKeycode(dpy, XK_Num_Lock)) {
				numlockmask = (1 << i);
			}
		}
	}
	XFreeModifiermap(modmap);
}
void updatestatus() {
	if (! gettextprop(root, XA_WM_NAME, stext, sizeof(stext))) { strcpy(stext, "dwm-Me"); }
	drawbar(selmon);
}
void view(const Arg *arg) {
	if ((arg->ui & TAGMASK) == selmon->tagset[ selmon->seltags ]) { return; }
	/* toggle sel tagset */
	selmon->seltags ^= 1;
	if (arg->ui & TAGMASK) { selmon->tagset[ selmon->seltags ] = arg->ui & TAGMASK; }
	Client::focus(nullptr);
	arrange(selmon);
}
Client *wintoclient(Window w) {
	Client *c;
	Monitor *m;
	for (m = mons; m; m = m->next) {
		for (c = m->clients; c; c = c->next) {
			if (c->win == w) { return c; }
		}
	}
	dwmLog::ENDFUNC(__func__);
	return nullptr;
}
Monitor *wintomon(Window w) {
	int x, y;
	Client *c;
	Monitor *m;

	if (w == root && getrootptr(&x, &y)) { return recttomon(x, y, 1, 1); }
	for (m = mons; m; m = m->next) {
		if (w == m->barwin) { return m; }
		if ((c = wintoclient(w))) { return c->mon; }
	}
	return selmon;
}
int xerror(Display *dpy, XErrorEvent *ee) {
	if (ee->error_code == BadWindow ||
		(ee->request_code == X_SetInputFocus && ee->error_code == BadMatch) ||
		(ee->request_code == X_PolyText8 && ee->error_code == BadDrawable) ||
		(ee->request_code == X_PolyFillRectangle && ee->error_code == BadDrawable) ||
		(ee->request_code == X_PolySegment && ee->error_code == BadDrawable) ||
		(ee->request_code == X_ConfigureWindow && ee->error_code == BadMatch) ||
		(ee->request_code == X_GrabButton && ee->error_code == BadAccess) ||
		(ee->request_code == X_GrabKey && ee->error_code == BadAccess) ||
		(ee->request_code == X_CopyArea && ee->error_code == BadDrawable))
	{ return 0; }
	fprintf(stderr, "dwm: fatal error: request code=%d, error code=%d\n", ee->request_code, ee->error_code);
	return xerrorxlib(dpy, ee); /* may call exit */
}
int xerrordummy(Display *dpy, XErrorEvent *ee) {
	return 0;
}
int xerrorstart(Display *dpy, XErrorEvent *ee) {
	die("dwm: another window manager is already running");
	return -1;
}
/* my functions */
void shiftview(const Arg *arg) {
	Arg shifted;
	unsigned int seltag = selmon->tagset[ selmon->seltags ];
	int i, num_tags = LENGTH(tags), cur_tag = -1;

	/*Find the current tag*/
	for (i = 0; i < num_tags; ++i) {
		if (seltag & (1 << i)) {
			cur_tag = i;
			break;
		}
	}
	/*Calculate the new tag*/
	if (cur_tag != -1) {
		cur_tag = (cur_tag + arg->i + num_tags) % num_tags;
		shifted.ui = (1 << cur_tag);
		view(&shifted);
	}
}
void shiftview_app(const Arg *arg) {
	Arg shifted;
	unsigned int seltag = selmon->tagset[ selmon->seltags ];
	int i, num_tags = LENGTH(tags), cur_tag = -1;

	/* Find the current tag */
	for (i = 0; i < num_tags; ++i) {
		if (seltag & (1 << i)) {
			cur_tag = i;
			break;
		}
	}
	/* Calculate the new tag */
	if (cur_tag != -1) {
		cur_tag = (cur_tag + arg->i + num_tags) % num_tags;
		shifted.ui = (1 << cur_tag);

		/* if no client is focused just view the tag */
		if (!selmon->sel){
			view(&shifted);
			return;
		}

		view_with_client(&shifted);
	}
}
void view_with_client(const Arg *arg) {
	if (!selmon) {
		return;
	}

	unsigned int newTags = arg->ui & TAGMASK;
	if (newTags) {
		selmon->tagset[ selmon->seltags ] = newTags;
		selmon->sel->tags = newTags;
		Client::focus(nullptr);
		arrange(selmon);
	}
}
void QUIT(const Arg *arg) {
	dwmLog::End();
	const char *KILL = "/bin/dwm-KILL";
	std::system(KILL);
}
#ifndef WMDECOR
#define WMDECOR
Window
createTitleBar(Client *c) {
	Window titleBar;
	int height = 20;
	titleBar = XCreateSimpleWindow(dpy, c->win, 0, 0, c->w, height, 1, 0, 0);
	return titleBar;
}
void applyWinDecorations(Client *c) {
	c->tB.win = createTitleBar(c);
	XMapWindow(dpy, c->tB.win);
}
#endif /* WMDECOR */
/* end of my functions */
int main(int argc, char *argv[]) {
	if (argc == 2 && ! strcmp ("-v", argv[1])) {
		die ("dwm-M");
	} else if (argc != 1) {
		die("usage: dwm [-v]");
	}
	if (!setlocale(LC_CTYPE, "") || !XSupportsLocale()) {
		fputs("warning: no locale support\n", stderr);
	}
	if (!(dpy = XOpenDisplay(nullptr))) {
		die("dwm: cannot open display");
	}
	dwmLog::Start();
	checkotherwm ();
	setup ();
	scan ();
	run ();
	cleanup ();
	XCloseDisplay ( dpy );
	return EXIT_SUCCESS;
}
