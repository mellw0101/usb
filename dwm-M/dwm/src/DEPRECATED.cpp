/*
 *
 *
 *
 * arg > 1.0 will set mfact absolutely
 *
 *
 *
"" setmfact	"" DEPRECATED */
/*
void setmfact ( const Arg *arg ) {
	float f;
	if ( ! arg || ! selmon->lt[ selmon->sellt ]->arrange )
		return;
	f = arg->f < 1.0 ? arg->f + selmon->mfact : arg->f - 1.0;
	if ( f < 0.05 || f > 0.95 )
		return;
	selmon->mfact = f;
	arrange ( selmon );
}
*/

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

/*	"" tile "" DEPRECATED FOR NOW
void tile ( Monitor *m ) {
	unsigned int i, n, h, mw, my, ty;
	Client *c;

	for ( n = 0, c = nexttiled ( m->clients ); c; c = nexttiled ( c->next ), n++ ) {}
	if ( n == 0 )
		return;
	if ( n > m->nmaster )
		mw = m->nmaster ? m->ww * m->mfact : 0;
	else
		mw = m->ww;
	for ( i = my = ty = 0, c = nexttiled ( m->clients ); c; c = nexttiled ( c->next ), i++ ) {
		if ( i < m->nmaster ) {
			h = ( m->wh - my ) / ( Min ( n, m->nmaster ) - i );
			resize ( c, m->wx, m->wy + my, mw - ( 2 * c->bw ), h - ( 2 * c->bw ), 0 );
			if ( my + HEIGHT ( c ) < m->wh )
				my += HEIGHT ( c );
		}
		else {
			h = ( m->wh - ty ) / ( n - i );
			resize ( c, m->wx + mw, m->wy + ty, m->ww - mw - ( 2 * c->bw ), h - ( 2 * c->bw ), 0 );
			if ( ty + HEIGHT ( c ) < m->wh )
				ty += HEIGHT ( c );
		}
	}
}
*/
/*
static void setmfact ( const Arg *arg );
static void tile ( Monitor *m );
Bool isCursorNearBorder ( Client *c, int proximity ) {
	int x, y;
	// Get cursor position
	if ( ! getrootptr ( &x, &y ) )
		return false;

	int distFromLeft = x - c->x;
	int distFromRight = ( c->x + c->w ) - x;
	int distFromTop =
		y - c->y;
	int
		distFromBottom =
			( c->y + c->h ) - y;

	if (
		distFromLeft <= proximity ||
		distFromRight <= proximity ||
		distFromTop <= proximity ||
		distFromBottom <= proximity ) {
		return True;// Cursor is close to any border
	}
	return False;// Cursor is not close to any border
}
static Bool isCursorNearBorder ( Client *c, int proximity );
Bool on = False;
void movemouseIfNearBorder ( const Arg *arg );
static Bool nearBorder = False;
static int ROOT_mouse_X = 0;
static int ROOT_mouse_Y = 0;
static int MOTION_ROOT_mouse_X = 0;
static int MOTION_ROOT_mouse_Y = 0;
const int edgeProximity = 10;// Pixels near the edge
#include "FileHandler/FileHandler.hpp"
static std::string logFILE = "/home/mellw/log";

static void
log ( const std::string &input ) {
	FileHandler file ( logFILE );

	if ( file.open () ) {
		file.append ( input + "\n" );
	}
}
static const char dmenufont[] = "monospace:size=12";

static const char *xterm[] = {
	"/bin/xterm",
	nullptr

	};
static const char
*konsole[] =
{
	"/usr/bin/konsole",
	NULL
};
static const char *termcmd[] = {
			"st",
			nullptr
	};

	static char dmenumon[ 2 ] = "0"; // component of dmenucmd, manipulated in spawn()
static const char *dmenucmd[] = {
			"dmenu_run",
			"-m",
			dmenumon,
			"-fn",
			dmenufont,
			"-nb",
			col_gray1,
			"-nf",
			col_gray3,
			"-sb",
			col_cyan,
			"-sf",
			col_gray4,
			nullptr

};

void zoom ( const Arg *arg ) {
	Client *c = selmon->sel;
	if ( ! selmon->lt[ selmon->sellt ]->arrange || ! c || c->isfloating )
		return;
	if ( c == nexttiled ( selmon->clients ) && ! ( c = nexttiled ( c->next ) ) )
		return;
	pop ( c );
}
void toggleview ( const Arg *arg ) {
	unsigned int newtagset = selmon->tagset[ selmon->seltags ] ^ ( arg->ui & TAGMASK );

	if ( newtagset ) {
		selmon->tagset[ selmon->seltags ] = newtagset;
		focus ( nullptr );
		arrange ( selmon );
	}
}
void toggletag ( const Arg *arg ) {
	unsigned int newtags;
	if ( ! selmon->sel )
		return;
	newtags = selmon->sel->tags ^ ( arg->ui & TAGMASK );

	if ( newtags ) {
		selmon->sel->tags = newtags;
		focus ( nullptr );
		arrange ( selmon );
	}
}
void tagmon ( const Arg *arg ) {
	if ( ! selmon->sel || ! mons->next )
		return;
	sendmon ( selmon->sel, dirtomon ( arg->i ) );
}
void tag ( const Arg *arg ) {
	if ( selmon->sel && arg->ui & TAGMASK ) {
		selmon->sel->tags = arg->ui & TAGMASK;
		focus ( nullptr );
		arrange ( selmon );
	}
}
static void zoom ( const Arg *arg );
static void toggletag ( const Arg *arg );
static void toggleview ( const Arg *arg );
static void tagmon ( const Arg *arg );
static void tag ( const Arg *arg );
static void setlayout ( const Arg *arg );
static void monocle ( Monitor *m );
static void incnmaster ( const Arg *arg );
static void focusmon ( const Arg *arg );




void pop ( Client *c ) {
	detach ( c );
	attach ( c );
	focus ( c );
	arrange ( c->mon );
}


void focusmon ( const Arg *arg ) {
	if ( ! mons || ! mons->next )
		return;
	Monitor *m = dirtomon ( arg->i );
	if ( selmon && m && m != selmon ) {
		unfocus ( selmon->sel, 0 );
		selmon = m;
		focus ( nullptr );
	}
}

void incnmaster ( const Arg *arg ) {
	selmon->nmaster = MAX ( selmon->nmaster + arg->i, 0 );
	arrange ( selmon );
}

static void pop ( Client *c );
Client *nexttiled ( Client *c ) {
	for ( ; c && ( c->isfloating || ! ISVISIBLE ( c ) ); c = c->next ) {}
	return c;
}

static Client *nexttiled ( Client *c );
static Monitor *dirtomon ( int dir );
Monitor *dirtomon ( int dir ) {
	Monitor *m = nullptr;
	if ( dir > 0 ) {
		if ( ! ( m = selmon->next ) )
			m = mons;
	}
	else if ( selmon == mons )
		for ( m = mons; m->next; m = m->next ) {}
	else
		for ( m = mons; m->next != selmon; m = m->next ) {}
	return m;
}


void monocle ( Monitor *m ) {
	unsigned int n = 0;
	Client *c;

	for ( c = m->clients; c; c = c->next )
		if ( ISVISIBLE ( c ) )
			n++;
	if ( n > 0 ) //override layout symbol
		snprintf ( m->ltsymbol, sizeof m->ltsymbol, "[%d]", n );
	for ( c = nexttiled ( m->clients ); c; c = nexttiled ( c->next ) )
		resize ( c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0 );
}


	TAGKEYS ( XK_1, 0 )
	TAGKEYS ( XK_2, 1 )
	TAGKEYS ( XK_3, 2 )
	TAGKEYS ( XK_4, 3 )
	TAGKEYS ( XK_5, 4 )
	TAGKEYS ( XK_6, 5 )
	TAGKEYS ( XK_7, 6 )
	TAGKEYS ( XK_8, 7 )
	TAGKEYS ( XK_9, 8 )

	{ ALT, XK_p, spawn, { .v = dmenucmd } },
	{ ALT | SHIFT, XK_Return, spawn, { .v = termcmd } },
	{ 	ALT | SUPER, 		R, 			focusstack, 		{ .i = +1 } 			},
	{ 	ALT | SUPER, 		L, 			focusstack, 		{ .i = -1 } 			},
	{ ALT, XK_i, incnmaster, { .i = +1 } },
	{ ALT, XK_d, incnmaster, { .i = -1 } },
	{ ALT, XK_h, setmfact, { .f = -0.05 } },
	{ ALT, XK_l, setmfact, { .f = +0.05 } },
	{ ALT, XK_Return, zoom, { 0 } },
	{ 	ALT, 				XK_Tab, 	view, 				{ 0 } 					},

	{ ALT, XK_t, setlayout, { .v = &layouts[ 0 ] } },
	{ ALT, XK_f, setlayout, { .v = &layouts[ 1 ] } },
	{ ALT, XK_m, setlayout, { .v = &layouts[ 2 ] } },
	{ ALT, XK_space, setlayout, { 0 } },
	{ ALT | SHIFT, XK_space, togglefloating, { 0 } },
	{ ALT, XK_comma, focusmon, { .i = -1 } },
	{ ALT, XK_period, focusmon, { .i = +1 } },
	{ ALT | SHIFT, XK_comma, tagmon, { .i = -1 } },
	{ ALT | SHIFT, XK_period, tagmon, { .i = +1 } },




*/

/*
void manage(Window w, XWindowAttributes *wa) {
	dwmLog::FUNC(__func__);
	Client *c, *t = nullptr;
	Window trans = None;
	XWindowChanges wc;
	c = static_cast<Client *>(ecalloc(1, sizeof(Client)));

	define_win_for_client(c, wa, w);
	Client::updatetitle(c);
	applyWinDecorations(c);
	getTrans(c, t, w, trans);
	checkAndMakeWinFit(c);
	c->bw = borderpx;
	wc.border_width = c->bw;
	XConfigureWindow(dpy, w, CWBorderWidth, &wc);
	XSetWindowBorder(dpy, scheme[ SchemeNorm ][ ColBorder ].pixel, w);
	Client::configure(c);  // propagates border_width, if size doesn't change
	Client::updatewindowtype(c);
	Client::updatesizehints(c);
	Client::updatewmhints(c);
	XSelectInput(dpy, w, EnterWindowMask | FocusChangeMask | PropertyChangeMask | StructureNotifyMask);
	Client::grabbuttons(c, 0);
	checkFloating(c, trans);
	Client::attach(c);
	Client::attachstack(c);
	// some windows require this
	XChangeProperty(dpy, root, netatom[ NetClientList ], XA_WINDOW, 32, PropModeAppend, ( unsigned char * ) &(c->win), 1);
	XMoveResizeWindow(dpy, c->win, c->x + 2 * sw, c->y, c->w, c->h);
	Client::setclientstate(c, NormalState);

	if (c->mon == selmon) {
		Client::unfocus(selmon->sel, 0);
	}

	c->mon->sel = c;
	arrange(c->mon);
	XMapWindow(dpy, c->win);
	Client::focus(nullptr);
}
*/

/*
static void frameWindow(Client *c) {
	int titlebarHeight = 20; // Adjust as needed
	// Create frame window
	c->frame = XCreateSimpleWindow(dpy, root, c->x, c->y, c->w, c->h + titlebarHeight, 0, BlackPixel(dpy, screen), WhitePixel(dpy, screen));
	// Reparent the client window to the frame
	XReparentWindow(dpy, c->win, c->frame, 0, titlebarHeight);
	// Adjust client window size to fit in the frame
	XResizeWindow(dpy, c->win, c->w, c->h);
	// Map the frame and titlebar
	XMapWindow(dpy, c->frame);
	// Save frame window ID in client structure
	c->program = c->win;
	c->win = c->frame;
	XSelectInput(dpy, c->win, EnterWindowMask | FocusChangeMask | PropertyChangeMask | StructureNotifyMask);
}
*/
