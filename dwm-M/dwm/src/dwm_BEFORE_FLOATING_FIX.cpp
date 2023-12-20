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

#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif /* XINERAMA */
#include <X11/Xft/Xft.h>

#include "drw.hpp"
#include "util.hpp"

/* macros */
#define BUTTONMASK ( ButtonPressMask | ButtonReleaseMask )
#define CLEANMASK( mask )                                                     \
	( mask & ~( numlockmask | LockMask ) &                                    \
	  ( ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | \
		Mod5Mask ) )
#define INTERSECT( x, y, w, h, m )                                                         \
	( MAX ( 0, MIN ( ( x ) + ( w ), ( m )->wx + ( m )->ww ) - MAX ( ( x ), ( m )->wx ) ) * \
	  MAX ( 0, MIN ( ( y ) + ( h ), ( m )->wy + ( m )->wh ) - MAX ( ( y ), ( m )->wy ) ) )
#define ISVISIBLE( C ) ( ( C->tags & C->mon->tagset[ C->mon->seltags ] ) )
#define LENGTH( X ) ( sizeof X / sizeof X[ 0 ] )
#define MOUSEMASK ( BUTTONMASK | PointerMotionMask )
#define WIDTH( X ) ( ( X )->w + 2 * ( X )->bw )
#define HEIGHT( X ) ( ( X )->h + 2 * ( X )->bw )
#define TAGMASK ( ( 1 << LENGTH ( tags ) ) - 1 )
#define TEXTW( X ) ( drw_fontset_getwidth ( drw, ( X ) ) + lrpad )

/* enums */
enum { CurNormal,
	   CurResize,
	   CurMove,
	   CurLast }; /* cursor */
// enum color_schemes { SchemeNorm, SchemeSel }; /* color schemes */
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
	ClickedBorder
}; /* clicks */

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

typedef struct Monitor Monitor;
typedef struct Client Client;

struct Client {
	char name[ 256 ];
	float mina, maxa;
	int x, y, w, h;
	int oldx, oldy, oldw, oldh;
	int basew, baseh, incw, inch, maxw, maxh, minw, minh, hintsvalid;
	int bw, oldbw;
	unsigned int tags;
	int isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen;
	Client *next;
	Client *snext;
	Monitor *mon;
	Window win;
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

struct Monitor {
	char ltsymbol[ 16 ];
	float mfact;
	int nmaster;
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

typedef struct {
	const char *class_name;
	const char *instance;
	const char *title;
	unsigned int tags;
	int isfloating;
	int monitor;
} Rule;

/*
typedef class Monitor Monitor;
typedef class Client Client;
class Client {
public:
    // Constructor
    Client();

    // Methods for Client
    // (Add methods as needed, for example, to update client state, manage
windows, etc.)


    char name[256];
    float mina, maxa;
    int x, y, w, h;
    int oldx, oldy, oldw, oldh;
    int basew, baseh, incw, inch, maxw, maxh, minw, minh, hintsvalid;
    int bw, oldbw;
    unsigned int tags;
    int isfixed, isfloating, isurgent, neverfocus, isfullscreen;
    int oldstate;
    Client* next;
    Client* snext;
    Monitor* mon; // Assuming Monitor is defined
    Window win;
};

class Monitor {
public:
    Monitor(); // Constructor

        char ltsymbol[16];
    float mfact;
    int nmaster, num, by;
    int mx, my, mw, mh; // screen size
    int wx, wy, ww, wh; // window area
    unsigned int seltags, sellt;
    unsigned int tagset[2];
    int showbar;
        int topbar;
    Client* clients;
    Client* sel;
    Client* stack;
    Monitor* next;
    Window barwin;
    const Layout* lt[2];
};
*/

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

static void shiftview ( const Arg *arg );
static void shiftview_app ( const Arg *arg );
static void view_with_client ( const Arg *arg );

#	endif /* FUNCTION_DECLARATIONS */

#	ifndef M_CUSTOM
#define M_CUSTOM

#include "SafeConverter.hpp"

#	endif /* M_CUSTOM */

#	ifndef variables
#define variables
static const char broken[] = "broken";
static char stext[ 256 ];
static int screen;
static int sw, sh; /* X display screen geometry width, height */
static int bh;     /* bar height */
static int lrpad;  /* sum of left and right padding for text */
static int ( *xerrorxlib ) ( Display *, XErrorEvent * );
static unsigned int numlockmask = 0;
static void ( *handler[ LASTEvent ] ) ( XEvent * ) = {
	[ButtonPressMask] = buttonpress,
	[ClientMessage] = clientmessage,
	[ConfigureRequest] = configurerequest,
	[ConfigureNotify] = configurenotify,
	[DestroyNotify] = destroynotify,
	[EnterNotify] = enternotify,
	[Expose] = expose,
	[FocusIn] = focusin,
	[KeyPress] = keypress,
	[MappingNotify] = mappingnotify,
	[MapRequest] = maprequest,
	[MotionNotify] = motionnotify,
	[PropertyNotify] = propertynotify,
	[UnmapNotify] = unmapnotify };
static Atom wmatom[ WMLast ], netatom[ NetLast ];
static int running = 1;
static Cur *cursor[ CurLast ];
static Clr **scheme;
static Display *dpy;
static Drw *drw;
static Monitor *mons, *selmon;
static Window root, wmcheckwin;
#	endif /* variables */

#	ifndef CONFIG
#define CONFIG

#include <X11/X.h>

static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 0;            /* 0 means bottom bar */
static const char *fonts[] = { "monospace:size=12" };

enum ColorSchemeIndex { SchemeNorm,
						SchemeSel };

struct ColorScheme {
	const char *fg;
	const char *bg;
	const char *border;
};

static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_cyan[] = "#005577";

static const ColorScheme colors[] = {
	{ // SchemeNorm
	  col_gray3, col_gray1, col_gray2 },
	{ // SchemeSel
	  col_gray4, col_cyan, col_cyan },
};

static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/*  xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
	/* 	class      		instance    	title       tags
       mask isfloating monitor 	*/
	{ nullptr, nullptr, nullptr, 0, 1, -1 },
	{ "Gimp", nullptr, nullptr, 0, 1, -1 },
	/*
            { 	"Firefox",		nullptr, 		nullptr, 	1
       << 8, 0, 				-1 			},
            */
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
	1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
	1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	{ "><>", nullptr }, /* no layout function means floating behavior */
						/*	{ "[]=", 		tile },    first entry is default
{ "[M]", 		monocle },*/
};

#	ifndef KEY_DEFENITIONS /* key definitions */
#define KEY_DEFENITIONS

#	ifndef KEYS
#define KEYS /* KEYS */

#define ALT Mod1Mask
#define SUPER Mod4Mask
#define CTRL ControlMask
#define SHIFT ShiftMask
#define TAB XK_Tab
#define L XK_Left
#define R XK_Right
#define T XK_t
#define Q XK_q

#	endif /* KEYS */

#	ifndef TAGKEYS
#define TAGKEYS( KEY, TAG )	\
	{ ALT, KEY, 				view, 		{ .ui = 1 << ( TAG ) } },   \
	{ ALT | CTRL, KEY, 			toggleview, { .ui = 1 << ( TAG ) } },	\
	{ ALT | SHIFT, KEY, 		tag, 		{ .ui = 1 << ( TAG ) } },   \
	{ ALT | CTRL | SHIFT, KEY, 	toggletag, 	{ .ui = 1 << ( TAG ) } },

#   endif /* TAGKEYS */

#	ifndef SHIFTVIEW

#define SHIFTVIEW( KEY, TAG )	\
	{ SUPER | CTRL, KEY, 			shiftview, 		{ .i = ( TAG ) } }, \
	{ SUPER | SHIFT | CTRL, KEY, 	shiftview_app, 	{ .i = ( TAG ) } },

#   endif /* SHIFTVIEW */

#	ifndef SPAWN

#define SPAWN( MOD, KEY, TAG ) { MOD, KEY, spawn, { .v = ( TAG ) } },

#   endif /* SPAWN */

#	endif /* KEY_DEFENITIONS */

#	ifndef COMMANDS
#define COMMANDS

static const char *dwm_KILL[] 	= 	{ "/usr/bin/dwm-KILL", nullptr };
static const char *alacritty[] 	= 	{ "/usr/bin/alacritty", nullptr };

#	endif /* COMMANDS */

static const Key keys[] = {
	/*	modifier			key			function  	argument 	*/
	{ 	ALT, 				XK_b, 		togglebar, 	{ 0 } 		},
	{ 	ALT | SHIFT, 		XK_c, 		killclient, { 0 } 		},
	{ 	ALT, 				TAB, 		focusstack, { .i = +1 } },
	SPAWN ( ALT | CTRL, T, alacritty )
	SPAWN ( ALT | SHIFT, Q, dwm_KILL )
	SHIFTVIEW ( L, -1 ) SHIFTVIEW ( R, +1 )
};

/* button definitions */
static const Button buttons[] = {
		/*	click			event mask		button 		function  		argument 	*/
		{ 	ClkClientWin, 	ALT, 			Button1, 	movemouse, 		{ 0 } 		},
		{ 	ClkClientWin, 	ALT, 			Button3, 	resizemouse, 	{ 0 } 		},
};

/* compile-time check if all tags fit into an unsigned int bit array. */
struct NumTags {
	char limitexceeded[ LENGTH ( tags ) > 31 ? -1 : 1 ];
};
#endif /* CONFIG */

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
	XGetClassHint ( dpy, c->win, &ch );
	class_name = ch.res_class ? ch.res_class : broken;
	instance = ch.res_name ? ch.res_name : broken;

	for ( i = 0; i < LENGTH ( rules ); i++ ) {
		r = &rules[ i ];

		if ( ( ! r->title || strstr ( c->name, r->title ) ) &&
			 ( ! r->class_name || strstr ( class_name, r->class_name ) ) &&
			 ( ! r->instance || strstr ( instance, r->instance ) ) ) {
			c->isfloating = r->isfloating;
			c->tags |= r->tags;

			for ( m = mons; m && m->num != r->monitor; m = m->next ) {
			}
			if ( m )
				c->mon = m;
		}
	}
	if ( ch.res_class )
		XFree ( ch.res_class );
	if ( ch.res_name )
		XFree ( ch.res_name );
	c->tags =
		c->tags & TAGMASK ? c->tags & TAGMASK : c->mon->tagset[ c->mon->seltags ];
}
int applysizehints ( Client *c, int *x, int *y, int *w, int *h, int interact ) {
	int baseismin;
	Monitor *m = c->mon;

	/* set minimum possible */
	*w = MAX ( 1, *w );
	*h = MAX ( 1, *h );

	if ( interact ) {
		if ( *x > sw )
			*x = sw - WIDTH ( c );
		if ( *y > sh )
			*y = sh - HEIGHT ( c );
		if ( *x + *w + 2 * c->bw < 0 )
			*x = 0;
		if ( *y + *h + 2 * c->bw < 0 )
			*y = 0;
	}
	else {
		if ( *x >= m->wx + m->ww )
			*x = m->wx + m->ww - WIDTH ( c );
		if ( *y >= m->wy + m->wh )
			*y = m->wy + m->wh - HEIGHT ( c );
		if ( *x + *w + 2 * c->bw <= m->wx )
			*x = m->wx;
		if ( *y + *h + 2 * c->bw <= m->wy )
			*y = m->wy;
	}

	if ( *h < bh )
		*h = bh;
	if ( *w < bh )
		*w = bh;

	if ( resizehints || c->isfloating || ! c->mon->lt[ c->mon->sellt ]->arrange ) {
		if ( ! c->hintsvalid )
			updatesizehints ( c );

		/* see last two sentences in ICCCM 4.1.2.3 */
		baseismin = c->basew == c->minw && c->baseh == c->minh;

		/* temporarily remove base dimensions */
		if ( ! baseismin ) {
			*w -= c->basew;
			*h -= c->baseh;
		}

		/* adjust for aspect limits */
		if ( c->mina > 0 && c->maxa > 0 ) {
			float aspect_ratio_w =
				static_cast<float> ( *h ) * c->maxa; /* calculating aspect_ratio using
                                               max and min area client can be */
			float aspect_ratio_h = static_cast<float> ( *w ) * c->mina;

			if ( c->maxa < static_cast<float> ( *w ) / static_cast<float> ( *h ) ) {
				*w = static_cast<int> ( std::round ( aspect_ratio_w ) );
			}
			else if ( c->mina < static_cast<float> ( *h ) / static_cast<float> ( *w ) ) {
				*h = static_cast<int> ( std::round ( aspect_ratio_h ) );
			}
		}

		/* increment calculation requires this */
		if ( baseismin ) {
			*w -= c->basew;
			*h -= c->baseh;
		}

		/* adjust for increment value */
		if ( c->incw )
			*w -= *w % c->incw;
		if ( c->inch )
			*h -= *h % c->inch;

		/* restore base dimensions */
		*w = MAX ( *w + c->basew, c->minw );
		*h = MAX ( *h + c->baseh, c->minh );
		if ( c->maxw )
			*w = MIN ( *w, c->maxw );
		if ( c->maxh )
			*h = MIN ( *h, c->maxh );
	}
	return *x != c->x || *y != c->y || *w != c->w || *h != c->h;
}
/*	"" applysizehints "" OLD
 *	THE CODE BELOW IS OLD AND DEPRECATED
 *
 *	// adjust for aspect limits
 *	if ( c->mina > 0 && c->maxa > 0 ) {
 * 	if ( c->maxa < ( float ) *w / *h ) *w = *h * c->maxa + 0.5;
 * 	else if ( c->mina < ( float ) *h / *w )
 * 			*h = *w * c->mina + 0.5;
 * 	}
 *
 *
 */
void arrange ( Monitor *m ) {
	Monitor *monitor = m ? m : mons;
	while ( monitor ) {
		showhide ( monitor->stack );
		if ( m ) {
			arrangemon ( monitor );
			restack ( monitor );
			break;
		}
		monitor = monitor->next;
	}
}
/*	"" arrange "" OLD
 * 	DEPRECATED
void
arrange(Monitor *m)
{
        if (m){
                showhide(m->stack);
        } else for (m = mons; m; m = m->next) {
                showhide(m->stack);
        }

        if (m) {
                arrangemon(m);
                restack(m);
        } else for (m = mons; m; m = m->next) {
                arrangemon(m);
        }
}
*/
void arrangemon ( Monitor *m ) {
	strncpy ( m->ltsymbol, m->lt[ m->sellt ]->symbol, sizeof m->ltsymbol );
	if ( m->lt[ m->sellt ]->arrange )
		m->lt[ m->sellt ]->arrange ( m );
}
void attach ( Client *c ) {
	c->next = c->mon->clients;
	c->mon->clients = c;
}
void attachstack ( Client *c ) {
	c->snext = c->mon->stack;
	c->mon->stack = c;
}
void buttonpress ( XEvent *e ) {
	unsigned int i, x, click;
	Arg arg = { 0 };
	Client *c;
	Monitor *m;
	XButtonPressedEvent *ev = &e->xbutton;
	click = ClkRootWin;

	/* focus monitor if necessary */
	if ( ( m = wintomon ( ev->window ) ) && m != selmon ) {
		unfocus ( selmon->sel, 1 );
		selmon = m;
		focus ( nullptr );
	}

	if ( ev->window == selmon->barwin ) {
		i = x = 0;
		do
			x += TEXTW ( tags[ i ] );
		while ( ev->x >= x && ++i < LENGTH ( tags ) );
		if ( i < LENGTH ( tags ) ) {
			click = ClkTagBar;
			arg.ui = 1 << i;
		}
		else if ( ev->x < x + TEXTW ( selmon->ltsymbol ) )
			click = ClkLtSymbol;
		else if ( ev->x > selmon->ww - ( int ) TEXTW ( stext ) )
			click = ClkStatusText;
		else
			click = ClkWinTitle;
	}
	else if ( ( c = wintoclient ( ev->window ) ) ) {
		focus ( c );
		restack ( selmon );
		XAllowEvents ( dpy, ReplayPointer, CurrentTime );
		click = ClkClientWin;
	}
	for ( i = 0; i < LENGTH ( buttons ); i++ ) {
		if ( click == buttons[ i ].click && buttons[ i ].func &&
			 buttons[ i ].button == ev->button &&
			 CLEANMASK ( buttons[ i ].mask ) == CLEANMASK ( ev->state ) )
			buttons[ i ].func (
				click == ClkTagBar && buttons[ i ].arg.i == 0 ? &arg : &buttons[ i ].arg );
	}
}
void checkotherwm () { /* this causes an error if some other window manager is
                         running */
	xerrorxlib = XSetErrorHandler ( xerrorstart );
	XSelectInput ( dpy, DefaultRootWindow ( dpy ), SubstructureRedirectMask );
	XSync ( dpy, False );
	XSetErrorHandler ( xerror );
	XSync ( dpy, False );
}
void cleanup () {
	Arg a = { .ui = static_cast<unsigned int> ( ~0 ) };
	Layout foo = { "", nullptr };
	Monitor *m;
	size_t i;
	view ( &a );
	selmon->lt[ selmon->sellt ] = &foo;

	for ( m = mons; m; m = m->next )
		while ( m->stack )
			unmanage ( m->stack, 0 );
	XUngrabKey ( dpy, AnyKey, AnyModifier, root );
	while ( mons )
		cleanupmon ( mons );
	for ( i = 0; i < CurLast; i++ )
		drw_cur_free ( drw, cursor[ i ] );
	for ( i = 0; i < LENGTH ( colors ); i++ )
		free ( scheme[ i ] );
	free ( scheme );
	XDestroyWindow ( dpy, wmcheckwin );
	drw_free ( drw );
	XSync ( dpy, False );
	XSetInputFocus ( dpy, PointerRoot, RevertToPointerRoot, CurrentTime );
	XDeleteProperty ( dpy, root, netatom[ NetActiveWindow ] );
}
void cleanupmon ( Monitor *mon ) {
	Monitor *m;
	if ( mon == mons )
		mons = mons->next;
	else {
		for ( m = mons; m && m->next != mon; m = m->next ) {
		}
		m->next = mon->next;
	}
	XUnmapWindow ( dpy, mon->barwin );
	XDestroyWindow ( dpy, mon->barwin );
	free ( mon );
}
void clientmessage ( XEvent *e ) {
	XClientMessageEvent *cme = &e->xclient;
	Client *c = wintoclient ( cme->window );

	if ( ! c )
		return;
	if ( cme->message_type == netatom[ NetWMState ] ) {
		if ( cme->data.l[ 1 ] == netatom[ NetWMFullscreen ] ||
			 cme->data.l[ 2 ] == netatom[ NetWMFullscreen ] )
			setfullscreen ( c, ( cme->data.l[ 0 ] == 1 /* _NET_WM_STATE_ADD    */ ||
								 ( cme->data.l[ 0 ] == 2 /* _NET_WM_STATE_TOGGLE */ &&
								   ! c->isfullscreen ) ) );
	}
	else if ( cme->message_type == netatom[ NetActiveWindow ] )
		if ( c != selmon->sel && ! c->isurgent )
			seturgent ( c, 1 );
}
void configure ( Client *c ) {
	if ( ! c || ! dpy )
		return;// Validation

	XConfigureEvent ce = { 0 };
	ce.type = ConfigureNotify;
	ce.display = dpy;
	ce.event = c->win;
	ce.window = c->win;
	ce.x = c->x;
	ce.y = c->y;
	ce.width = c->w;
	ce.height = c->h;
	ce.border_width = c->bw;
	ce.above = None;
	ce.override_redirect = False;

	XSendEvent ( dpy, c->win, False, StructureNotifyMask, ( XEvent * ) &ce );
	/* Status status =
  if (status == 0) {

  } */
}
void configurenotify ( XEvent *e ) {
	Monitor *m;
	Client *c;
	XConfigureEvent *ev = &e->xconfigure;
	int dirty;

	if ( ev->window ==
		 root ) { /* TODO: updategeom handling sucks, needs to be simplified */
		dirty = ( sw != ev->width || sh != ev->height );
		sw = ev->width;
		sh = ev->height;
		if ( updategeom () || dirty ) {
			drw_resize ( drw, sw, bh );
			updatebars ();
			for ( m = mons; m; m = m->next )
				for ( c = m->clients; c; c = c->next ) {
					if ( c->isfullscreen )
						resizeclient ( c, m->mx, m->my, m->mw, m->mh );
					XMoveResizeWindow ( dpy, m->barwin, m->wx, m->by, m->ww, bh );
				}
			focus ( nullptr );
			arrange ( nullptr );
		}
	}
}
void configurerequest ( XEvent *e ) {
	Client *c;
	Monitor *m;
	XConfigureRequestEvent *ev = &e->xconfigurerequest;
	XWindowChanges wc;

	if ( ( c = wintoclient ( ev->window ) ) ) {
		if ( ev->value_mask & CWBorderWidth )
			c->bw = ev->border_width;
		else if ( c->isfloating || ! selmon->lt[ selmon->sellt ]->arrange ) {
			m = c->mon;
			if ( ev->value_mask & CWX ) {
				c->oldx = c->x;
				c->x = m->mx + ev->x;
			}
			if ( ev->value_mask & CWY ) {
				c->oldy = c->y;
				c->y = m->my + ev->y;
			}
			if ( ev->value_mask & CWWidth ) {
				c->oldw = c->w;
				c->w = ev->width;
			}
			if ( ev->value_mask & CWHeight ) {
				c->oldh = c->h;
				c->h = ev->height;
			}
			if ( ( c->x + c->w ) > m->mx + m->mw && c->isfloating )
				c->x = m->mx + ( m->mw / 2 - WIDTH ( c ) / 2 ); /* center in x direction */
			if ( ( c->y + c->h ) > m->my + m->mh && c->isfloating )
				c->y = m->my + ( m->mh / 2 - HEIGHT ( c ) / 2 ); /* center in y direction */
			if ( ( ev->value_mask & ( CWX | CWY ) ) &&
				 ! ( ev->value_mask & ( CWWidth | CWHeight ) ) )
				configure ( c );
			if ( ISVISIBLE ( c ) )
				XMoveResizeWindow ( dpy, c->win, c->x, c->y, c->w, c->h );
		}
		else
			configure ( c );
	}
	else {
		wc.x = ev->x;
		wc.y = ev->y;
		wc.width = ev->width;
		wc.height = ev->height;
		wc.border_width = ev->border_width;
		wc.sibling = ev->above;
		wc.stack_mode = ev->detail;
		XConfigureWindow ( dpy, ev->window, ev->value_mask, &wc );
	}
	XSync ( dpy, False );
}

Monitor *createmon () {
	std::unique_ptr<Monitor> m = std::make_unique<Monitor> ();

	m->tagset[ 0 ] = m->tagset[ 1 ] = 1;
	m->mfact = mfact;
	m->nmaster = nmaster;
	m->showbar = showbar;
	m->topbar = topbar;
	m->lt[ 0 ] = &layouts[ 0 ];
	m->lt[ 1 ] = &layouts[ 1 % LENGTH ( layouts ) ];

	strncpy ( m->ltsymbol, layouts[ 0 ].symbol, sizeof ( m->ltsymbol ) );
	return m.release ();// Release ownership and return the raw pointer
}
void destroynotify ( XEvent *e ) {
	Client *c;
	XDestroyWindowEvent *ev = &e->xdestroywindow;

	if ( ( c = wintoclient ( ev->window ) ) )
		unmanage ( c, 1 );
}
void detach ( Client *c ) {
	if ( ! c || ! c->mon )
		return;
	Client **tc;
	for ( tc = &c->mon->clients; *tc && *tc != c; tc = &( *tc )->next ) {
	}
	*tc = c->next;
}
void detachstack ( Client *c ) {
	if ( ! c || ! c->mon )
		return;
	Client **tc, *t;

	for ( tc = &c->mon->stack; *tc && *tc != c; tc = &( *tc )->snext ) {
	}
	*tc = c->snext;

	if ( c == c->mon->sel ) {
		for ( t = c->mon->stack; t && ! ISVISIBLE ( t ); t = t->snext ) {
		}
		c->mon->sel = t;
	}
}
void drawbar ( Monitor *m ) {
	int x, w, tw = 0;
	int boxs = SafeConverter::UnsignedToInt ( drw->fonts->h ) / 9;
	int boxw = SafeConverter::UnsignedToInt ( drw->fonts->h ) / 6 + 2;
	unsigned int i, occ = 0, urg = 0;
	Client *c;

	if ( ! m->showbar )
		return;
	/* draw status first, so it can be overdrawn by tags later
   * status is only drawn on selected monitor */
	if ( m == selmon ) {
		drw_setscheme ( drw, scheme[ SchemeNorm ] );
		tw = TEXTW ( stext ) - lrpad + 2; /* 2px right padding */
		drw_text ( drw, m->ww - tw, 0, tw, bh, 0, stext, 0 );
	}
	for ( c = m->clients; c; c = c->next ) {
		occ |= c->tags;
		if ( c->isurgent )
			urg |= c->tags;
	}
	x = 0;
	for ( i = 0; i < LENGTH ( tags ); i++ ) {
		w = TEXTW ( tags[ i ] );
		drw_setscheme (
			drw, scheme[ m->tagset[ m->seltags ] & 1 << i ? SchemeSel : SchemeNorm ] );
		drw_text ( drw, x, 0, w, bh, lrpad / 2, tags[ i ],
				   SafeConverter::UnsignedToInt ( urg ) & 1 << i );
		if ( occ & 1 << i )
			drw_rect ( drw, x + boxs, boxs, boxw, boxw,
					   m == selmon && selmon->sel && selmon->sel->tags & 1 << i,
					   SafeConverter::UnsignedToInt ( urg ) & 1 << i );
		x += w;
	}
	w = TEXTW ( m->ltsymbol );
	drw_setscheme ( drw, scheme[ SchemeNorm ] );
	x = drw_text ( drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0 );

	if ( ( w = m->ww - tw - x ) > bh ) {
		if ( m->sel ) {
			drw_setscheme ( drw, scheme[ m == selmon ? SchemeSel : SchemeNorm ] );
			drw_text ( drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0 );
			if ( m->sel->isfloating )
				drw_rect ( drw, x + boxs, boxs, boxw, boxw, m->sel->isfixed, 0 );
		}
		else {
			drw_setscheme ( drw, scheme[ SchemeNorm ] );
			drw_rect ( drw, x, 0, w, bh, 1, 1 );
		}
	}
	drw_map ( drw, m->barwin, 0, 0, m->ww, bh );
}
void drawbars () {
	Monitor *m;
	for ( m = mons; m; m = m->next )
		drawbar ( m );
}
void enternotify ( XEvent *e ) {
	/* nearBorder = true; */

	Client *c;
	Monitor *m;
	XCrossingEvent *ev = &e->xcrossing;

	if ( ( ev->mode != NotifyNormal || ev->detail == NotifyInferior ) &&
		 ev->window != root )
		return;
	c = wintoclient ( ev->window );
	m = c ? c->mon : wintomon ( ev->window );

	if ( m != selmon ) {
		unfocus ( selmon->sel, 1 );
		selmon = m;
	}
	else if ( ! c || c == selmon->sel )
		return;
	focus ( c );
}
void expose ( XEvent *e ) {
	XExposeEvent *ev = &e->xexpose;
	Monitor *m = wintomon ( ev->window );
	if ( m && ev->count == 0 )
		drawbar ( m );
}
void focus ( Client *c ) {
	if ( ! c || ! ISVISIBLE ( c ) ) {
		c = selmon->stack;
		while ( c && ! ISVISIBLE ( c ) )
			c = c->snext;
	}
	if ( selmon->sel && selmon->sel != c )
		unfocus ( selmon->sel, 0 );
	if ( c ) {
		if ( c->mon != selmon )
			selmon = c->mon;
		if ( c->isurgent )
			seturgent ( c, 0 );
		detachstack ( c );
		attachstack ( c );
		grabbuttons ( c, 1 );
		XSetWindowBorder ( dpy, c->win, scheme[ SchemeSel ][ ColBorder ].pixel );
		setfocus ( c );
	}
	else {
		XSetInputFocus ( dpy, root, RevertToPointerRoot, CurrentTime );
		XDeleteProperty ( dpy, root, netatom[ NetActiveWindow ] );
	}
	selmon->sel = c;
	drawbars ();
}
/*	"" focus "" OLD
 * 	DEPRECATED
void
focus
(Client *c)
{
        if (!c || !ISVISIBLE(c)){
        for (c = selmon->stack; c && !ISVISIBLE(c); c = c->snext) {}
    }

        if (selmon->sel && selmon->sel != c) {
        unfocus(selmon->sel, 0);
    }

    if (c) {
        if (c->mon != selmon) {
            selmon = c->mon;
        }

        if (c->isurgent) {
            seturgent(c, 0);
        }

                detachstack(c);
                attachstack(c);
                grabbuttons(c, 1);
                XSetWindowBorder
                (
                        dpy,
                        c->win,
                        scheme[SchemeSel][ColBorder].pixel
                );
                setfocus(c);
        } else {
                XSetInputFocus( dpy, root, RevertToPointerRoot, CurrentTime );

                XDeleteProperty
                (
                        dpy,
                        root,
                        netatom[NetActiveWindow]
                );
        }

        selmon->sel = c;
        drawbars();
}
*/
void focusin ( XEvent *e ) { /* there are some broken focus acquiring clients
                             needing extra handling */
	XFocusChangeEvent *ev = &e->xfocus;

	if ( selmon->sel && ev->window != selmon->sel->win ) {
		setfocus ( selmon->sel );
	}
}
void focusstack ( const Arg *arg ) {
	if ( ! selmon->sel || ( selmon->sel->isfullscreen & lockfullscreen ) )
		return;
	Client *next_client = nullptr;
	if ( arg->i > 0 ) {
		for ( next_client = selmon->sel->next ? selmon->sel->next : selmon->clients;
			  next_client && ! ISVISIBLE ( next_client );
			  next_client = next_client->next ? next_client->next
											  : selmon->clients ) {}
	}
	else {
		for ( Client *i = selmon->clients; i;
			  i = i->next ) /* Find the previous visible client before the currently
                         selected client. */
			if ( ISVISIBLE ( i ) ) {
				next_client = i;
				if ( i == selmon->sel )
					break;
			}
	}
	if ( next_client ) {
		focus ( next_client );
		restack ( selmon );
	}
}
/* 	"" focusstack "" OLD
 *	THIS IS OLD "" focusstack "" CODE AND IS DEPRECATED
 *

 *	Find the next visible client after the currently selected client.
 * 	If not found, start from the beginning of the client list.

for (next_client = selmon->sel->next; next_client && !ISVISIBLE(next_client);
next_client = next_client->next) {}

if (!next_client) {
        for (next_client = selmon->clients; next_client &&
!ISVISIBLE(next_client); next_client = next_client->next) {}
}







void
focusstack
(const Arg *arg)
{
        Client *c = nullptr, *i;

        if (!selmon->sel || (selmon->sel->isfullscreen & lockfullscreen)) {
        return;
    }

        if (arg->i > 0) {
        for (c = selmon->sel->next; c && !ISVISIBLE(c); c = c->next) {}

        if (!c) {
            for (c = selmon->clients; c && !ISVISIBLE(c); c = c->next) {}
        }
    } else {
        for (i = selmon->clients; i != selmon->sel; i = i->next) {
            if (ISVISIBLE(i)) {
                c = i;
            }
        }

        if (!c) {
            for (; i; i = i->next) {
                if (ISVISIBLE(i)) {
                    c = i;
                }
            }
        }
        }

    if (c) {
                focus(c);
                restack(selmon);
        }
}
*/
Atom getatomprop ( Client *c, Atom prop ) {
	int di;
	unsigned long dl;
	unsigned char *p = nullptr;
	Atom da, atom = None;

	if ( XGetWindowProperty ( dpy, c->win, prop, 0L, sizeof atom, False, XA_ATOM,
							  &da, &di, &dl, &dl, &p ) == Success &&
		 p ) {
		atom = *( Atom * ) p;
		XFree ( p );
	}
	return atom;
}
int getrootptr ( int *x, int *y ) {
	int di;
	unsigned int dui;
	Window dummy;
	return XQueryPointer ( dpy, root, &dummy, &dummy, x, y, &di, &di, &dui );
}
long getstate ( Window w ) {
	int format;
	long result = -1;
	unsigned char *p = nullptr;
	unsigned long n, extra;
	Atom real;

	if ( XGetWindowProperty ( dpy, w, wmatom[ WMState ], 0L, 2L, False,
							  wmatom[ WMState ], &real, &format, &n, &extra,
							  ( unsigned char ** ) &p ) != Success )
		return -1;
	if ( n != 0 )
		result = *p;
	XFree ( p );
	return result;
}
int gettextprop ( Window w, Atom atom, char *text, unsigned int size ) {
	char **list = nullptr;
	int n;
	XTextProperty name;

	if ( ! text || size == 0 )
		return 0;
	text[ 0 ] = '\0';
	if ( ! XGetTextProperty ( dpy, w, &name, atom ) || ! name.nitems )
		return 0;
	if ( name.encoding == XA_STRING )
		strncpy ( text, ( char * ) name.value, size - 1 );
	else if ( XmbTextPropertyToTextList ( dpy, &name, &list, &n ) >= Success &&
			  n > 0 && *list ) {
		strncpy ( text, *list, size - 1 );
		XFreeStringList ( list );
	}
	text[ size - 1 ] = '\0';
	XFree ( name.value );
	return 1;
}
void grabbuttons ( Client *c, int focused ) {
	updatenumlockmask ();
	{
		unsigned int i, j;
		unsigned int modifiers[] = { 0, LockMask, numlockmask,
									 numlockmask | LockMask };

		XUngrabButton ( dpy, AnyButton, AnyModifier, c->win );
		if ( ! focused )
			XGrabButton ( dpy, AnyButton, AnyModifier, c->win, False, BUTTONMASK,
						  GrabModeSync, GrabModeSync, None, None );
		for ( i = 0; i < LENGTH ( buttons ); i++ )
			if ( buttons[ i ].click == ClkClientWin )
				for ( j = 0; j < LENGTH ( modifiers ); j++ )
					XGrabButton ( dpy, buttons[ i ].button, buttons[ i ].mask | modifiers[ j ],
								  c->win, False, BUTTONMASK, GrabModeAsync, GrabModeSync,
								  None, None );
	}
}
void grabkeys () {
	updatenumlockmask ();
	{
		unsigned int i, j, k;
		unsigned int modifiers[] = { 0, LockMask, numlockmask,
									 numlockmask | LockMask };
		int start, end, skip;
		KeySym *syms;

		XUngrabKey ( dpy, AnyKey, AnyModifier, root );
		XDisplayKeycodes ( dpy, &start, &end );
		syms = XGetKeyboardMapping ( dpy, start, end - start + 1, &skip );
		if ( ! syms )
			return;
		for ( k = start; k <= end; k++ )
			for ( i = 0; i < LENGTH ( keys ); i++ )
				if ( keys[ i ].keysym == syms[ ( k - start ) * skip ] )
					for ( j = 0; j < LENGTH ( modifiers ); j++ )
						XGrabKey ( dpy, SafeConverter::UnsignedToInt ( k ),
								   keys[ i ].mod | modifiers[ j ], root, True, GrabModeAsync,
								   GrabModeAsync );
		XFree ( syms );
	}
}
void keypress ( XEvent *e ) {
	unsigned int i;
	KeySym keysym;
	XKeyEvent *ev;
	ev = &e->xkey;

	/* Updated function call */
	keysym = XkbKeycodeToKeysym ( dpy, ( KeyCode ) ev->keycode, 0, 0 );
	for ( i = 0; i < LENGTH ( keys ); i++ )
		if ( keysym == keys[ i ].keysym &&
			 CLEANMASK ( keys[ i ].mod ) == CLEANMASK ( ev->state ) && keys[ i ].func )
			keys[ i ].func ( &( keys[ i ].arg ) );
}
void killclient ( const Arg *arg ) {
	if ( ! selmon->sel )
		return;
	if ( ! sendevent ( selmon->sel, wmatom[ WMDelete ] ) ) {
		XGrabServer ( dpy );
		XSetErrorHandler ( xerrordummy );
		XSetCloseDownMode ( dpy, DestroyAll );
		XKillClient ( dpy, selmon->sel->win );
		XSync ( dpy, False );
		XSetErrorHandler ( xerror );
		XUngrabServer ( dpy );
	}
}
void manage ( Window w, XWindowAttributes *wa ) {
	Client *c, *t = nullptr;
	Window trans = None;
	XWindowChanges wc;

	c = static_cast<Client *> ( ecalloc ( 1, sizeof ( Client ) ) );
	c->win = w;
	/* geometry */
	c->x = c->oldx = wa->x;
	c->y = c->oldy = wa->y;
	c->w = c->oldw = wa->width;
	c->h = c->oldh = wa->height;
	c->oldbw = wa->border_width;

	updatetitle ( c );
	if ( XGetTransientForHint ( dpy, w, &trans ) && ( t = wintoclient ( trans ) ) ) {
		c->mon = t->mon;
		c->tags = t->tags;
	}
	else {
		c->mon = selmon;
		applyrules ( c );
	}
	if ( c->x + WIDTH ( c ) > c->mon->wx + c->mon->ww )
		c->x = c->mon->wx + c->mon->ww - WIDTH ( c );
	if ( c->y + HEIGHT ( c ) > c->mon->wy + c->mon->wh )
		c->y = c->mon->wy + c->mon->wh - HEIGHT ( c );
	c->x = MAX ( c->x, c->mon->wx );
	c->y = MAX ( c->y, c->mon->wy );
	c->bw = borderpx;
	wc.border_width = c->bw;
	XConfigureWindow ( dpy, w, CWBorderWidth, &wc );
	XSetWindowBorder ( dpy, scheme[ SchemeNorm ][ ColBorder ].pixel, w );
	configure ( c ); /* propagates border_width, if size doesn't change */
	updatewindowtype ( c );
	updatesizehints ( c );
	updatewmhints ( c );
	XSelectInput ( dpy, w,
				   EnterWindowMask | FocusChangeMask | PropertyChangeMask |
					   StructureNotifyMask );
	grabbuttons ( c, 0 );
	if ( ! c->isfloating ) {
		if ( trans == None )
			c->isfloating = c->oldstate = ! ! c->isfixed;
		else
			c->isfloating = c->oldstate = true;
	}
	if ( c->isfloating )
		XRaiseWindow ( dpy, c->win );
	attach ( c );
	attachstack ( c );
	XChangeProperty ( dpy, root, netatom[ NetClientList ], XA_WINDOW, 32,
					  PropModeAppend, ( unsigned char * ) &( c->win ), 1 );
	/* some windows require this */
	XMoveResizeWindow ( dpy, c->win, c->x + 2 * sw, c->y, c->w, c->h );
	setclientstate ( c, NormalState );
	if ( c->mon == selmon )
		unfocus ( selmon->sel, 0 );
	c->mon->sel = c;
	arrange ( c->mon );
	XMapWindow ( dpy, c->win );
	focus ( nullptr );
}
void mappingnotify ( XEvent *e ) {
	XMappingEvent *ev = &e->xmapping;
	XRefreshKeyboardMapping ( ev );
	if ( ev->request == MappingKeyboard )
		grabkeys ();
}
void maprequest ( XEvent *e ) {
	static XWindowAttributes wa;
	XMapRequestEvent *ev = &e->xmaprequest;

	if ( ! XGetWindowAttributes ( dpy, ev->window, &wa ) || wa.override_redirect )
		return;
	if ( ! wintoclient ( ev->window ) )
		manage ( ev->window, &wa );
}
void motionnotify ( XEvent *e ) {
	static Monitor *mon = nullptr;
	Monitor *m;
	XMotionEvent *ev = &e->xmotion;

	if ( ev->window != root )
		return;
	if ( ( m = recttomon ( ev->x_root, ev->y_root, 1, 1 ) ) != mon && mon ) {
		unfocus ( selmon->sel, 1 );
		selmon = m;
		focus ( nullptr );
	}
	mon = m;
}
void movemouse ( const Arg *arg ) {
	int x, y, ocx, ocy, nx, ny;
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if ( ! ( c = selmon->sel ) )
		return;
	if ( c->isfullscreen ) /* no support moving fullscreen windows by mouse */
		return;
	restack ( selmon );
	ocx = c->x;
	ocy = c->y;
	if ( XGrabPointer ( dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
						None, cursor[ CurMove ]->cursor, CurrentTime ) != GrabSuccess )
		return;
	if ( ! getrootptr ( &x, &y ) )
		return;
	do {
		XMaskEvent ( dpy, MOUSEMASK | ExposureMask | SubstructureRedirectMask, &ev );
		switch ( ev.type ) {
			case ConfigureRequest:
			case Expose:
			case MapRequest:
				handler[ ev.type ]( &ev );
				break;
			case MotionNotify:
				if ( ( ev.xmotion.time - lasttime ) <= ( 1000 / 240 ) )
					continue;
				lasttime = ev.xmotion.time;

				nx = ocx + ( ev.xmotion.x - x );
				ny = ocy + ( ev.xmotion.y - y );
				if ( abs ( selmon->wx - nx ) < snap )
					nx = selmon->wx;
				else if ( abs ( ( selmon->wx + selmon->ww ) - ( nx + WIDTH ( c ) ) ) < snap )
					nx = selmon->wx + selmon->ww - WIDTH ( c );
				if ( abs ( selmon->wy - ny ) < snap )
					ny = selmon->wy;
				else if ( abs ( ( selmon->wy + selmon->wh ) - ( ny + HEIGHT ( c ) ) ) < snap )
					ny = selmon->wy + selmon->wh - HEIGHT ( c );
				if ( ! c->isfloating && selmon->lt[ selmon->sellt ]->arrange &&
					 ( abs ( nx - c->x ) > snap || abs ( ny - c->y ) > snap ) )
					togglefloating ( nullptr );
				if ( ! selmon->lt[ selmon->sellt ]->arrange || c->isfloating )
					resize ( c, nx, ny, c->w, c->h, 1 );
				break;
		}
	} while ( ev.type != ButtonRelease );
	XUngrabPointer ( dpy, CurrentTime );
	if ( ( m = recttomon ( c->x, c->y, c->w, c->h ) ) != selmon ) {
		sendmon ( c, m );
		selmon = m;
		focus ( nullptr );
	}
}
void propertynotify ( XEvent *e ) {
	Client *c;
	Window trans;
	XPropertyEvent *ev = &e->xproperty;

	if ( ( ev->window == root ) && ( ev->atom == XA_WM_NAME ) )
		updatestatus ();
	else if ( ev->state == PropertyDelete )
		return; /* ignore */
	else if ( ( c = wintoclient ( ev->window ) ) ) {
		switch ( ev->atom ) {
			default:
				break;
			case XA_WM_TRANSIENT_FOR:
				if ( ! c->isfloating && ( XGetTransientForHint ( dpy, c->win, &trans ) ) &&
					 ( c->isfloating = ( wintoclient ( trans ) ) != nullptr ) )
					arrange ( c->mon );
				break;
			case XA_WM_NORMAL_HINTS:
				c->hintsvalid = 0;
				break;
			case XA_WM_HINTS:
				updatewmhints ( c );
				drawbars ();
				break;
		}
		if ( ev->atom == XA_WM_NAME || ev->atom == netatom[ NetWMName ] ) {
			updatetitle ( c );
			if ( c == c->mon->sel )
				drawbar ( c->mon );
		}
		if ( ev->atom == netatom[ NetWMWindowType ] )
			updatewindowtype ( c );
	}
}
Monitor *recttomon ( int x, int y, int w, int h ) {
	Monitor *m, *r = selmon;
	int a, area = 0;
	for ( m = mons; m; m = m->next )
		if ( ( a = INTERSECT ( x, y, w, h, m ) ) > area ) {
			area = a;
			r = m;
		}
	return r;
}
void resize ( Client *c, int x, int y, int w, int h, int interact ) {
	if ( applysizehints ( c, &x, &y, &w, &h, interact ) )
		resizeclient ( c, x, y, w, h );
}
void resizeclient ( Client *c, int x, int y, int w, int h ) {
	XWindowChanges wc;

	c->oldx = c->x;
	c->x = wc.x = x;
	c->oldy = c->y;
	c->y = wc.y = y;
	c->oldw = c->w;
	c->w = wc.width = w;
	c->oldh = c->h;
	c->h = wc.height = h;
	wc.border_width = c->bw;
	XConfigureWindow ( dpy, c->win, CWX | CWY | CWWidth | CWHeight | CWBorderWidth,
					   &wc );
	configure ( c );
	XSync ( dpy, False );
}
void resizemouse ( const Arg *arg ) {
	int ocx, ocy, nw, nh;
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if ( ! ( c = selmon->sel ) )
		return;
	if ( c->isfullscreen )
		return; /* no support resizing fullscreen windows by mouse */
	restack ( selmon );
	ocx = c->x;
	ocy = c->y;
	if ( XGrabPointer ( dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
						None, cursor[ CurResize ]->cursor, CurrentTime ) != GrabSuccess )
		return;
	XWarpPointer ( dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1,
				   c->h + c->bw - 1 );
	do {
		XMaskEvent ( dpy, MOUSEMASK | ExposureMask | SubstructureRedirectMask, &ev );
		switch ( ev.type ) {
			case ConfigureRequest:
			case Expose:
			case MapRequest:
				handler[ ev.type ]( &ev );
				break;
			case MotionNotify:
				if ( ( ev.xmotion.time - lasttime ) <= ( 1000 / 240 ) )
					continue;
				lasttime = ev.xmotion.time;
				nw = MAX ( ev.xmotion.x - ocx - 2 * c->bw + 1, 1 );
				nh = MAX ( ev.xmotion.y - ocy - 2 * c->bw + 1, 1 );
				if ( c->mon->wx + nw >= selmon->wx &&
					 c->mon->wx + nw <= selmon->wx + selmon->ww &&
					 c->mon->wy + nh >= selmon->wy &&
					 c->mon->wy + nh <= selmon->wy + selmon->wh )
					if ( ! c->isfloating && selmon->lt[ selmon->sellt ]->arrange &&
						 ( abs ( nw - c->w ) > snap || abs ( nh - c->h ) > snap ) )
						togglefloating ( nullptr );
				if ( ! selmon->lt[ selmon->sellt ]->arrange || c->isfloating )
					resize ( c, c->x, c->y, nw, nh, 1 );
				break;
		}
	} while ( ev.type != ButtonRelease );
	XWarpPointer ( dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1,
				   c->h + c->bw - 1 );
	XUngrabPointer ( dpy, CurrentTime );
	while ( XCheckMaskEvent ( dpy, EnterWindowMask, &ev ) ) {
	}
	if ( ( m = recttomon ( c->x, c->y, c->w, c->h ) ) != selmon ) {
		sendmon ( c, m );
		selmon = m;
		focus ( nullptr );
	}
}
void restack ( Monitor *m ) {
	Client *c;
	XEvent ev;
	XWindowChanges wc;
	drawbar ( m );

	if ( ! m->sel )
		return;
	if ( m->sel->isfloating || ! m->lt[ m->sellt ]->arrange )
		XRaiseWindow ( dpy, m->sel->win );
	if ( m->lt[ m->sellt ]->arrange ) {
		wc.stack_mode = Below;
		wc.sibling = m->barwin;

		for ( c = m->stack; c; c = c->snext )
			if ( ! c->isfloating && ISVISIBLE ( c ) ) {
				XConfigureWindow ( dpy, c->win, CWSibling | CWStackMode, &wc );
				wc.sibling = c->win;
			}
	}
	XSync ( dpy, False );
	while ( XCheckMaskEvent ( dpy, EnterWindowMask, &ev ) ) {}
}
#ifndef DEBUG_TOOLS
#define DEBUG_TOOLS
/*
bool ButtonClicked ( XEvent *e ) {
        if ( e->type == ButtonPress ) {
                Window clickedWindow = e->xbutton.window;
                Client *c = wintoclient ( clickedWindow );
                Log::INFO ( "ButtonClicked" );
                return true;
        }
        return false;
}
bool ClientClicked ( XEvent *e ) {
        if ( e->type == ClkClientWin ) {
                Window clickedWindow = e->xbutton.window;
                Client *c = wintoclient ( clickedWindow );
                Log::INFO ( "ClientClicked" );
                return true;
        }
        return false;
}
bool StatusTextClicked ( XEvent *e ) {
        if ( e->type == ClkStatusText ) {
                Window clickedWindow = e->xbutton.window;
                Client *c = wintoclient ( clickedWindow );
                Log::INFO ( "StatusTextClicked" );
                return true;
        }
        return false;
}
bool RootWindowClicked ( XEvent *e ) {
        if ( e->type == ClkRootWin ) {
                Window clickedWindow = e->xbutton.window;
                Client *c = wintoclient ( clickedWindow );
                Log::INFO ( "RootWindowClicked" );
                return true;
        }
        return false;
}
bool WasWindowClicked ( XEvent *e ) {
        Window clickedWindow = e->xbutton.window;
        Client *c = wintoclient ( clickedWindow );
        if ( c ) {
                Log::INFO ( "WasWindowClicked" );
                return true;
        }
        return false;
}
*/
#endif /* DEBUG_TOOLS */
/*
 *	This is the "Main event loop"
 *
"" run "" DESCRIPTON*/
void run () {
	XEvent ev;

	while ( running ) {
		XNextEvent ( dpy, &ev );
		if ( handler[ ev.type ] )
			handler[ ev.type ]( &ev );
	}
}
void scan () {
	unsigned int i, num;
	Window d1, d2, *wins = nullptr;
	XWindowAttributes wa;

	if ( XQueryTree ( dpy, root, &d1, &d2, &wins, &num ) ) {
		for ( i = 0; i < num; i++ ) {
			if ( ! XGetWindowAttributes ( dpy, wins[ i ], &wa ) || wa.override_redirect ||
				 XGetTransientForHint ( dpy, wins[ i ], &d1 ) )
				continue;
			if ( wa.map_state == IsViewable || getstate ( wins[ i ] ) == IconicState )
				manage ( wins[ i ], &wa );
		}
		for ( i = 0; i < num; i++ ) { /* now the transients */
			if ( ! XGetWindowAttributes ( dpy, wins[ i ], &wa ) )
				continue;
			if ( XGetTransientForHint ( dpy, wins[ i ], &d1 ) &&
				 ( wa.map_state == IsViewable || getstate ( wins[ i ] ) == IconicState ) )
				manage ( wins[ i ], &wa );
		}
		if ( wins )
			XFree ( wins );
	}
}
void sendmon ( Client *c, Monitor *m ) {
	if ( c->mon == m )
		return;
	unfocus ( c, 1 );
	detach ( c );
	detachstack ( c );
	c->mon = m;
	c->tags = m->tagset[ m->seltags ]; /* assign tags of target monitor */
	attach ( c );
	attachstack ( c );
	focus ( nullptr );
	arrange ( nullptr );
}
void setclientstate ( Client *c, long state ) {
	long data[] = { state, None };
	XChangeProperty ( dpy, c->win, wmatom[ WMState ], wmatom[ WMState ], 32,
					  PropModeReplace, ( unsigned char * ) data, 2 );
}
int sendevent ( Client *c, Atom proto ) {
	int n;
	Atom *protocols;
	int exists = 0;
	XEvent ev;

	if ( XGetWMProtocols ( dpy, c->win, &protocols, &n ) ) {
		while ( ! exists && n-- )
			exists = protocols[ n ] == proto;
		XFree ( protocols );
	}
	if ( exists ) {
		ev.type = ClientMessage;
		ev.xclient.window = c->win;
		ev.xclient.message_type = wmatom[ WMProtocols ];
		ev.xclient.format = 32;
		ev.xclient.data.l[ 0 ] = proto;
		ev.xclient.data.l[ 1 ] = CurrentTime;
		XSendEvent ( dpy, c->win, False, NoEventMask, &ev );
	}
	return exists;
}
void setfocus ( Client *c ) {
	if ( ! c->neverfocus ) {
		XSetInputFocus ( dpy, c->win, RevertToPointerRoot, CurrentTime );
		XChangeProperty ( dpy, root, netatom[ NetActiveWindow ], XA_WINDOW, 32,
						  PropModeReplace, ( unsigned char * ) &( c->win ), 1 );
	}
	sendevent ( c, wmatom[ WMTakeFocus ] );
}
void setfullscreen ( Client *c, int fullscreen ) {
	if ( fullscreen && ! c->isfullscreen ) {
		XChangeProperty ( dpy, c->win, netatom[ NetWMState ], XA_ATOM, 32,
						  PropModeReplace, ( unsigned char * ) &netatom[ NetWMFullscreen ],
						  1 );
		c->isfullscreen = 1;
		c->oldstate = c->isfloating;
		c->oldbw = c->bw;
		c->bw = 0;
		c->isfloating = 1;
		resizeclient ( c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh );
		XRaiseWindow ( dpy, c->win );
	}
	else if ( ! fullscreen && c->isfullscreen ) {
		XChangeProperty ( dpy, c->win, netatom[ NetWMState ], XA_ATOM, 32,
						  PropModeReplace, ( unsigned char * ) nullptr, 0 );
		c->isfullscreen = 0;
		c->isfloating = c->oldstate;
		c->bw = c->oldbw;
		c->x = c->oldx;
		c->y = c->oldy;
		c->w = c->oldw;
		c->h = c->oldh;
		resizeclient ( c, c->x, c->y, c->w, c->h );
		arrange ( c->mon );
	}
}
/*
void setlayout ( const Arg *arg ) {
        if ( ! arg || ! arg->v || arg->v != selmon->lt[ selmon->sellt ] )
                selmon->sellt ^= 1;
        if ( arg && arg->v )
                selmon->lt[ selmon->sellt ] = ( Layout * ) arg->v;
        strncpy ( selmon->ltsymbol, selmon->lt[ selmon->sellt ]->symbol, sizeof
selmon->ltsymbol ); if ( selmon->sel ) arrange ( selmon ); else drawbar ( selmon
);
}
*/
void setup () {
	int i;
	XSetWindowAttributes wa;
	Atom utf8string;
	struct sigaction sa {};
	/* do not transform children into zombies when they terminate */
	sigemptyset ( &sa.sa_mask );
	sa.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT | SA_RESTART;
	sa.sa_handler = SIG_IGN;
	sigaction ( SIGCHLD, &sa, nullptr );
	/* clean up any zombies (inherited from .xinitrc etc.) immediately */
	while ( waitpid ( -1, nullptr, WNOHANG ) > 0 ) {}
	/* init screen */
	screen = DefaultScreen ( dpy );
	sw = DisplayWidth ( dpy, screen );
	sh = DisplayHeight ( dpy, screen );
	root = RootWindow ( dpy, screen );
	drw = drw_create ( dpy, screen, root, sw, sh );
	if ( ! drw_fontset_create ( drw, fonts, LENGTH ( fonts ) ) )
		die ( "no fonts could be loaded." );
	lrpad = SafeConverter::UnsignedToInt ( drw->fonts->h );
	bh = SafeConverter::UnsignedToInt ( drw->fonts->h ) + 2;
	updategeom ();
	/* init atoms */
	utf8string = XInternAtom ( dpy, "UTF8_STRING", False );
	wmatom[ WMProtocols ] = XInternAtom ( dpy, "WM_PROTOCOLS", False );
	wmatom[ WMDelete ] = XInternAtom ( dpy, "WM_DELETE_WINDOW", False );
	wmatom[ WMState ] = XInternAtom ( dpy, "WM_STATE", False );
	wmatom[ WMTakeFocus ] = XInternAtom ( dpy, "WM_TAKE_FOCUS", False );
	netatom[ NetActiveWindow ] = XInternAtom ( dpy, "_NET_ACTIVE_WINDOW", False );
	netatom[ NetSupported ] = XInternAtom ( dpy, "_NET_SUPPORTED", False );
	netatom[ NetWMName ] = XInternAtom ( dpy, "_NET_WM_NAME", False );
	netatom[ NetWMState ] = XInternAtom ( dpy, "_NET_WM_STATE", False );
	netatom[ NetWMCheck ] = XInternAtom ( dpy, "_NET_SUPPORTING_WM_CHECK", False );
	netatom[ NetWMFullscreen ] =
		XInternAtom ( dpy, "_NET_WM_STATE_FULLSCREEN", False );
	netatom[ NetWMWindowType ] = XInternAtom ( dpy, "_NET_WM_WINDOW_TYPE", False );
	netatom[ NetWMWindowTypeDialog ] =
		XInternAtom ( dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False );
	netatom[ NetClientList ] = XInternAtom ( dpy, "_NET_CLIENT_LIST", False );
	/* init cursors */
	cursor[ CurNormal ] = drw_cur_create ( drw, XC_left_ptr );
	cursor[ CurResize ] = drw_cur_create ( drw, XC_sizing );
	cursor[ CurMove ] = drw_cur_create ( drw, XC_fleur );
	/* init appearance */
	scheme = static_cast<Clr **> ( ecalloc ( LENGTH ( colors ), sizeof ( Clr * ) ) );
	for ( i = 0; i < LENGTH ( colors ); i++ ) {
		const char *clrnames[] = { colors[ i ].fg, colors[ i ].bg, colors[ i ].border };
		scheme[ i ] = drw_scm_create (
			drw, clrnames, 3 );// Pass 3 because there are 3 colors in each scheme
	}
	/* init bars */
	updatebars ();
	updatestatus ();
	/* supporting window for NetWMCheck */
	wmcheckwin = XCreateSimpleWindow ( dpy, root, 0, 0, 1, 1, 0, 0, 0 );
	XChangeProperty ( dpy, wmcheckwin, netatom[ NetWMCheck ], XA_WINDOW, 32,
					  PropModeReplace, ( unsigned char * ) &wmcheckwin, 1 );
	XChangeProperty ( dpy, wmcheckwin, netatom[ NetWMName ], utf8string, 8,
					  PropModeReplace, ( unsigned char * ) "dwm", 3 );
	XChangeProperty ( dpy, root, netatom[ NetWMCheck ], XA_WINDOW, 32,
					  PropModeReplace, ( unsigned char * ) &wmcheckwin, 1 );
	/* EWMH support per view */
	XChangeProperty ( dpy, root, netatom[ NetSupported ], XA_ATOM, 32,
					  PropModeReplace, ( unsigned char * ) netatom, NetLast );
	XDeleteProperty ( dpy, root, netatom[ NetClientList ] );
	/* select events */
	wa.cursor = cursor[ CurNormal ]->cursor;
	wa.event_mask = SubstructureRedirectMask | SubstructureNotifyMask |
					ButtonPressMask | PointerMotionMask | EnterWindowMask |
					LeaveWindowMask | StructureNotifyMask | PropertyChangeMask;
	XChangeWindowAttributes ( dpy, root, CWEventMask | CWCursor, &wa );
	XSelectInput ( dpy, root, wa.event_mask );
	grabkeys ();
	focus ( nullptr );
}
void seturgent ( Client *c, int urg ) {
	XWMHints *wmh;
	c->isurgent = urg;
	if ( ! ( wmh = XGetWMHints ( dpy, c->win ) ) )
		return;
	wmh->flags = urg ? ( wmh->flags | XUrgencyHint ) : ( wmh->flags & ~XUrgencyHint );
	XSetWMHints ( dpy, c->win, wmh );
	XFree ( wmh );
}
void showhide ( Client *c ) {
	if ( ! c )
		return;
	if ( ISVISIBLE ( c ) ) /* show clients top down */
	{
		XMoveWindow ( dpy, c->win, c->x, c->y );
		if ( ( ! c->mon->lt[ c->mon->sellt ]->arrange || c->isfloating ) &&
			 ! c->isfullscreen )
			resize ( c, c->x, c->y, c->w, c->h, 0 );
		showhide ( c->snext );
	}
	else /* hide clients bottom up */
	{
		showhide ( c->snext );
		XMoveWindow ( dpy, c->win, WIDTH ( c ) * -2, c->y );
	}
}

void spawn ( const Arg *arg ) {
	struct sigaction sa {};
	/*
  if ( arg->v == dmenucmd ) dmenumon[ 0 ] = '0' + selmon->num;
  */
	if ( fork () == 0 ) {
		if ( dpy )
			close ( ConnectionNumber ( dpy ) );
		setsid ();
		sigemptyset ( &sa.sa_mask );
		sa.sa_flags = 0;
		sa.sa_handler = SIG_DFL;
		sigaction ( SIGCHLD, &sa, nullptr );
		execvp ( ( ( char ** ) arg->v )[ 0 ], ( char ** ) arg->v );
		die ( "shit FAILED: end !!" );
	}
}
void togglebar ( const Arg *arg ) {
	selmon->showbar = ! selmon->showbar;
	updatebarpos ( selmon );
	XMoveResizeWindow ( dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww,
						bh );
	arrange ( selmon );
}
void togglefloating ( const Arg *arg ) {
	if ( ! selmon->sel )
		return;
	if ( selmon->sel->isfullscreen )
		return; /* no support for fullscreen windows */
	selmon->sel->isfloating = ! selmon->sel->isfloating || selmon->sel->isfixed;
	if ( selmon->sel->isfloating )
		resize ( selmon->sel, selmon->sel->x, selmon->sel->y, selmon->sel->w,
				 selmon->sel->h, 0 );
	arrange ( selmon );
}
void unfocus ( Client *c, int setfocus ) {
	if ( ! c )
		return;
	grabbuttons ( c, 0 );
	XSetWindowBorder ( dpy, c->win, scheme[ SchemeNorm ][ ColBorder ].pixel );

	if ( setfocus ) {
		XSetInputFocus ( dpy, root, RevertToPointerRoot, CurrentTime );
		XDeleteProperty ( dpy, root, netatom[ NetActiveWindow ] );
	}
}
/*	"" unfocus "" OLD
 *
 * 	BELOW IS OLD CODE IT IS DEPRECATED
 *
void unfocus(Client *c, int setfocus) {
        if (!c) {
                return;
        }

        grabbuttons(c, 0);

        // Optimized Border Color Update: Only update if color is different
        static unsigned long lastBorderColor = 0;
        unsigned long newBorderColor = scheme[SchemeNorm][ColBorder].pixel;

        if (lastBorderColor != newBorderColor) {
                XSetWindowBorder(dpy, c->win, newBorderColor);
                lastBorderColor = newBorderColor;
        }

        // Set focus to root window if required
        if (setfocus) {
                static Window lastFocus = 0;
                if (lastFocus != root) {
                        XSetInputFocus(dpy, root, RevertToPointerRoot,
CurrentTime); lastFocus = root;
                }

                // Update active window property
                XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
        }
}
*/
void unmanage ( Client *c, int destroyed ) {
	Monitor *m = c->mon;
	XWindowChanges wc;
	detach ( c );
	detachstack ( c );
	if ( ! destroyed ) {
		wc.border_width = c->oldbw;
		/* avoid race conditions */
		XGrabServer ( dpy );
		XSetErrorHandler ( xerrordummy );
		XSelectInput ( dpy, c->win, NoEventMask );
		/* restore border */
		XConfigureWindow ( dpy, c->win, CWBorderWidth, &wc );
		XUngrabButton ( dpy, AnyButton, AnyModifier, c->win );
		setclientstate ( c, WithdrawnState );
		XSync ( dpy, False );
		XSetErrorHandler ( xerror );
		XUngrabServer ( dpy );
	}
	free ( c );
	focus ( nullptr );
	updateclientlist ();
	arrange ( m );
}
void unmapnotify ( XEvent *e ) {
	Client *c;
	XUnmapEvent *ev = &e->xunmap;

	if ( ( c = wintoclient ( ev->window ) ) ) {
		if ( ev->send_event )
			setclientstate ( c, WithdrawnState );
		else
			unmanage ( c, 0 );
	}
}
void updatebars () {
	Monitor *m;
	XSetWindowAttributes wa;
	wa.override_redirect = True;
	wa.background_pixmap = ParentRelative;
	wa.event_mask = ButtonPressMask | ExposureMask;
	char className[] = "dwm";
	char classTitle[] = "dwm";
	XClassHint ch = { className, classTitle };

	for ( m = mons; m; m = m->next ) {
		if ( m->barwin )
			continue;
		m->barwin = XCreateWindow (
			dpy, root, m->wx, m->by, m->ww, bh, 0, DefaultDepth ( dpy, screen ),
			CopyFromParent, DefaultVisual ( dpy, screen ),
			CWOverrideRedirect | CWBackPixmap | CWEventMask, &wa );
		XDefineCursor ( dpy, m->barwin, cursor[ CurNormal ]->cursor );
		XMapRaised ( dpy, m->barwin );
		XSetClassHint ( dpy, m->barwin, &ch );
	}
}
void updatebarpos ( Monitor *m ) {
	m->wy = m->my;
	m->wh = m->mh;

	if ( m->showbar ) {
		m->wh -= bh;
		m->by = m->topbar ? m->wy : m->wy + m->wh;
		m->wy = m->topbar ? m->wy + bh : m->wy;
	}
	else
		m->by = -bh;
}
void updateclientlist () {
	Client *c;
	Monitor *m;
	XDeleteProperty ( dpy, root, netatom[ NetClientList ] );

	for ( m = mons; m; m = m->next )
		for ( c = m->clients; c; c = c->next )
			XChangeProperty ( dpy, root, netatom[ NetClientList ], XA_WINDOW, 32,
							  PropModeAppend, ( unsigned char * ) &( c->win ), 1 );
}
int updategeom () {
	int dirty = 0;
	if ( ! mons )
		mons = createmon ();
	if ( mons->mw != sw || mons->mh != sh ) { /* Check if screen size has changed */
		dirty = 1;
		mons->mw = mons->ww = sw;
		mons->mh = mons->wh = sh;
		updatebarpos ( mons );
	}
	if ( dirty )
		selmon = wintomon ( root ); /* Directly set selmon to the result of wintomon */
	return dirty;
}
void updatenumlockmask () {
	unsigned int i, j;
	numlockmask = 0;
	XModifierKeymap *modmap;
	modmap = XGetModifierMapping ( dpy );

	for ( i = 0; i < 8; i++ )
		for ( j = 0; j < modmap->max_keypermod; j++ )
			if ( modmap->modifiermap[ i * modmap->max_keypermod + j ] ==
				 XKeysymToKeycode ( dpy, XK_Num_Lock ) )
				numlockmask = ( 1 << i );
	XFreeModifiermap ( modmap );
}
void updatesizehints ( Client *c ) {
	long msize;
	XSizeHints size;

	/* Initializing all fields to zero */
	memset ( &size, 0, sizeof ( XSizeHints ) );

	if ( ! XGetWMNormalHints ( dpy, c->win, &size, &msize ) )
		size.flags = PSize;
	bool hasBaseSize = size.flags & PBaseSize;
	bool hasMinSize = size.flags & PMinSize;
	bool hasMaxSize = size.flags & PMaxSize;
	bool hasResizeInc = size.flags & PResizeInc;
	bool hasAspect = size.flags & PAspect;

	// Default values
	c->basew = c->baseh = c->incw = c->inch = c->maxw = c->maxh = c->minw =
		c->minh = 0;
	c->maxa = c->mina = 0.0;

	if ( hasBaseSize ) {
		c->basew = size.base_width;
		c->baseh = size.base_height;
	}
	else if ( hasMinSize ) {
		c->basew = size.min_width;
		c->baseh = size.min_height;
	}

	if ( hasResizeInc ) {
		c->incw = size.width_inc;
		c->inch = size.height_inc;
	}

	if ( hasMaxSize ) {
		c->maxw = size.max_width;
		c->maxh = size.max_height;
	}

	if ( hasMinSize ) {
		c->minw = size.min_width;
		c->minh = size.min_height;
	}
	else if ( hasBaseSize ) {
		c->minw = size.base_width;
		c->minh = size.base_height;
	}
	if ( hasAspect ) {
		c->mina =
			( float ) size.min_aspect.y / SafeConverter::IntToFloat ( size.min_aspect.x );
		c->maxa =
			( float ) size.max_aspect.x / SafeConverter::IntToFloat ( size.max_aspect.y );
	}
	c->isfixed = ( c->maxw && c->maxh && c->maxw == c->minw && c->maxh == c->minh );
	c->hintsvalid = 1;
}
/*	"" updatesizehints "" OLD
 *
 *	BELOW IS OLD CODE IT IS DEPRECATED
 *
 * 	void
 * 	updatesizehints
 * 	(Client *c)
 *	{
 *	long msize;
 * 	XSizeHints size;
 * 	// size is uninitialized, ensure that size.flags aren't used /
 * 	if (!XGetWMNormalHints(dpy, c->win, &size, &msize)) {
 * 		size.flags = PSize;
 * 	}
 *
 *
 *
 *
 *
 *
 *
 *
 *











        if
        (size.flags & PBaseSize)
        {
                c->basew = size.base_width;
                c->baseh = size.base_height;
        }
        else if
        (size.flags & PMinSize)
        {
                c->basew = size.min_width;
                c->baseh = size.min_height;
        }
        else
        {
                c->basew = c->baseh = 0;
        }

        if
        (size.flags & PResizeInc)
        {
                c->incw = size.width_inc;
                c->inch = size.height_inc;
        }
        else
        {
                c->incw = c->inch = 0;
        }

        if
        (size.flags & PMaxSize)
        {
                c->maxw = size.max_width;
                c->maxh = size.max_height;
        }
        else
        {
                c->maxw = c->maxh = 0;
        }

        if
        (size.flags & PMinSize)
        {
                c->minw = size.min_width;
                c->minh = size.min_height;
        }
        else if
        (size.flags & PBaseSize)
        {
                c->minw = size.base_width;
                c->minh = size.base_height;
        }
        else
        {
                c->minw = c->minh = 0;
        }

        if (size.flags & PAspect) {
                c->mina = (float)size.min_aspect.y / size.min_aspect.x;
                c->maxa = (float)size.max_aspect.x / size.max_aspect.y;
        } else {
                c->maxa = c->mina = 0.0;
        }
        c->isfixed = (c->maxw && c->maxh && c->maxw == c->minw && c->maxh ==
c->minh); c->hintsvalid = 1;
}
*/
void updatestatus () {
	if ( ! gettextprop ( root, XA_WM_NAME, stext, sizeof ( stext ) ) )
		strcpy ( stext, "dwm-M" );
	drawbar ( selmon );
}
void updatetitle ( Client *c ) {
	if ( ! gettextprop ( c->win, netatom[ NetWMName ], c->name, sizeof c->name ) )
		gettextprop ( c->win, XA_WM_NAME, c->name, sizeof c->name );
	/* hack to mark broken clients */
	if ( c->name[ 0 ] == '\0' )
		strcpy ( c->name, broken );
}
void updatewindowtype ( Client *c ) {
	Atom state = getatomprop ( c, netatom[ NetWMState ] );
	Atom wtype = getatomprop ( c, netatom[ NetWMWindowType ] );

	if ( state == netatom[ NetWMFullscreen ] )
		setfullscreen ( c, 1 );
	if ( wtype == netatom[ NetWMWindowTypeDialog ] )
		c->isfloating = 1;
}
void updatewmhints ( Client *c ) {
	XWMHints *wmh;
	if ( ( wmh = XGetWMHints ( dpy, c->win ) ) ) {
		if ( c == selmon->sel && wmh->flags & XUrgencyHint ) {
			wmh->flags &= ~XUrgencyHint;
			XSetWMHints ( dpy, c->win, wmh );
		}
		else
			c->isurgent = ( wmh->flags & XUrgencyHint ) ? 1 : 0;
		if ( wmh->flags & InputHint )
			c->neverfocus = ! wmh->input;
		else
			c->neverfocus = 0;
		XFree ( wmh );
	}
}
void view ( const Arg *arg ) {
	if ( ( arg->ui & TAGMASK ) == selmon->tagset[ selmon->seltags ] )
		return;
	/* toggle sel tagset */
	selmon->seltags ^= 1;
	if ( arg->ui & TAGMASK )
		selmon->tagset[ selmon->seltags ] = arg->ui & TAGMASK;
	focus ( nullptr );
	arrange ( selmon );
}
Client *wintoclient ( Window w ) {
	Client *c;
	Monitor *m;
	for ( m = mons; m; m = m->next )
		for ( c = m->clients; c; c = c->next )
			if ( c->win == w )
				return c;
	return nullptr;
}
Monitor *wintomon ( Window w ) {
	int x, y;
	Client *c;
	Monitor *m;

	if ( w == root && getrootptr ( &x, &y ) )
		return recttomon ( x, y, 1, 1 );
	for ( m = mons; m; m = m->next ) {
		if ( w == m->barwin )
			return m;
		if ( ( c = wintoclient ( w ) ) )
			return c->mon;
	}
	return selmon;
}
/*
 * 	There's no way to check accesses to destroyed windows,
 *	thus those cases are ignored( especially on	UnmapNotify's ).
 *
 * 	Other types of errors call Xlibs default error handler, which may call
exit.
 *
"" xerror "" DESCRIPTON */
int xerror ( Display *dpy, XErrorEvent *ee ) {
	if ( ee->error_code == BadWindow ||
		 ( ee->request_code == X_SetInputFocus && ee->error_code == BadMatch ) ||
		 ( ee->request_code == X_PolyText8 && ee->error_code == BadDrawable ) ||
		 ( ee->request_code == X_PolyFillRectangle &&
		   ee->error_code == BadDrawable ) ||
		 ( ee->request_code == X_PolySegment && ee->error_code == BadDrawable ) ||
		 ( ee->request_code == X_ConfigureWindow && ee->error_code == BadMatch ) ||
		 ( ee->request_code == X_GrabButton && ee->error_code == BadAccess ) ||
		 ( ee->request_code == X_GrabKey && ee->error_code == BadAccess ) ||
		 ( ee->request_code == X_CopyArea && ee->error_code == BadDrawable ) )
		return 0;
	fprintf ( stderr, "dwm: fatal error: request code=%d, error code=%d\n",
			  ee->request_code, ee->error_code );
	return xerrorxlib ( dpy, ee ); /* may call exit */
}
int xerrordummy ( Display *dpy, XErrorEvent *ee ) { return 0; }
/*
 * 	Startup Error handler to check if another window manager
 * 	is already running.
 *
"" xerrorstart "" DESCRIPTON */
int xerrorstart ( Display *dpy, XErrorEvent *ee ) {
	die ( "dwm: another window manager is already running" );
	return -1;
}
void shiftview ( const Arg *arg ) {
	Arg shifted;
	unsigned int seltag = selmon->tagset[ selmon->seltags ];
	int i, num_tags = LENGTH ( tags ), cur_tag = -1;

	/*Find the current tag*/
	for ( i = 0; i < num_tags; ++i ) {
		if ( seltag & ( 1 << i ) ) {
			cur_tag = i;
			break;
		}
	}
	/*Calculate the new tag*/
	if ( cur_tag != -1 ) {
		cur_tag = ( cur_tag + arg->i + num_tags ) % num_tags;
		shifted.ui = ( 1 << cur_tag );
		view ( &shifted );
	}
}
void shiftview_app ( const Arg *arg ) {
	Arg shifted;
	unsigned int seltag = selmon->tagset[ selmon->seltags ];
	int i, num_tags = LENGTH ( tags ), cur_tag = -1;

	/*Find the current tag*/
	for ( i = 0; i < num_tags; ++i ) {
		if ( seltag & ( 1 << i ) ) {
			cur_tag = i;
			break;
		}
	}
	/*Calculate the new tag*/
	if ( cur_tag != -1 ) {
		cur_tag = ( cur_tag + arg->i + num_tags ) % num_tags;
		shifted.ui = ( 1 << cur_tag );
		view_with_client ( &shifted );
	}
}
void view_with_client ( const Arg *arg ) {
	if ( ! selmon )
		return;
	unsigned int newTags = arg->ui & TAGMASK;
	if ( newTags ) {
		selmon->tagset[ selmon->seltags ] = newTags;
		selmon->sel->tags = newTags;

		focus ( nullptr );
		arrange ( selmon );
	}
}
int main ( int argc, char *argv[] ) {
	if ( argc == 2 && ! strcmp ( "-v", argv[ 1 ] ) )
		die ( "dwm-M" );
	else if ( argc != 1 )
		die ( "usage: dwm [-v]" );
	if ( ! setlocale ( LC_CTYPE, "" ) || ! XSupportsLocale () )
		fputs ( "warning: no locale support\n", stderr );
	if ( ! ( dpy = XOpenDisplay ( nullptr ) ) )
		die ( "dwm: cannot open display" );
	checkotherwm ();
	setup ();
	scan ();
	run ();
	cleanup ();
	XCloseDisplay ( dpy );
	return EXIT_SUCCESS;
}
