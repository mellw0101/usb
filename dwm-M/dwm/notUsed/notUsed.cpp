// static const char**
// BASH_LAUNCHER(const char* cmd)
// {
// 	const char** args = new const char*[4];  // Allocate memory for 4 pointers
// 	args[0] = "/bin/bash";
// 	args[1] = "-c";
// 	args[2] = cmd;
// 	args[3] = NULL;
// 	return args;
// }
// static Bool
// isCursorInDesiredArea(const XButtonPressedEvent *e, int borderWidth)
// {
//     Client *c = wintoclient(e->window);
//     if (!c) return False;
//
//     int x = e->x;  // cursor x position relative to the window
//     int y = e->y;  // cursor y position relative to the window
//
//     // Check if the cursor is within the border width
//     if (x < borderWidth || x > (c->w - borderWidth) ||
//         y < borderWidth || y > (c->h - borderWidth)) {
//         return True; // cursor is within the border area
//     }
//
//     return False; // cursor is outside the border area
// }
// void
// movemouseIfNearBorder(const Arg *arg)
// {
//     Client *c = selmon->sel;
//     if (!c || c->isfullscreen) // No moving fullscreen windows by mouse
//         return;
//
//     nearBorder = isCursorNearBorder(c, arg->i);
//     if (nearBorder) {
//         movemouse(arg); // Call movemouse only if cursor is near the border
//     }
// }
// static Bool
// isCursorNearBorder(Client *c, int proximity)
// {
//     int x, y;
//     if (!getrootptr(&x, &y)) // Get cursor position
//         return False;
//
//     int distFromLeft = x - c->x;
//     int distFromRight = (c->x + c->w) - x;
//     int distFromTop = y - c->y;
//     int distFromBottom = (c->y + c->h) - y;
//
//     if (distFromLeft <= proximity || distFromRight <= proximity ||
//         distFromTop <= proximity || distFromBottom <= proximity) {
//         return True; // Cursor is close to any border
//     }
//
//     return False; // Cursor is not close to any border
// }
// { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
		// { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
		// { ClkWinTitle,          0,              Button2,        zoom,           {0} },
		// { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
		// { ClkClientWin,         ConfigureRequest,       	  	Button1,        movemouse,      {0} },
		// { ClkClientWin,         ConfigureRequest,         	Button1,        togglefloating, {0} },
		// { ClkClientWin,         ConfigureRequest,         	Button1,        resizemouse,    {0} },
		// { ClkTagBar,            0,              Button1,        view,           {0} },
		// { ClkTagBar,            0,              Button3,        toggleview,     {0} },
		// { ClkTagBar,            ALT,         	Button1,        tag,            {0} },
		// { ClkTagBar,            ALT,         	Button3,        toggletag,      {0} },

		// move windows when close to edges
		// { ClkClientWin,         0,              Button1,        movemouse,      {0} },


		// { ClkClientWin,         0,              Button1,        movemouseIfNearBorder, {.i = 10} },
		// { ClkWinTitle,         0,              Button1,        movemouse,      {0} },


	//		c->isfloating = c->oldstate = trans != None
	//		||
	//	c->isfixed;
		
		// c->isfloating = c->oldstate = !(trans == None && !c->isfixed);

// 		void setupButtonGrabs(Window win, int focused, unsigned int modifiers[]) {
//     XUngrabButton(dpy, AnyButton, AnyModifier, win);
//     if (!focused) {
//         XGrabButton(dpy, AnyButton, AnyModifier, win, False, BUTTONMASK, GrabModeSync, GrabModeSync, None, None);
//     }
//     for (unsigned int i = 0; i < LENGTH(buttons); i++) {
//         if (buttons[i].click == ClkClientWin || buttons[i].click == ClkWinTitle) { // Assuming ClkWinTitle is defined for title bar
//             for (unsigned int j = 0; j < LENGTH(modifiers); j++) {
//                 XGrabButton(dpy, buttons[i].button, buttons[i].mask | modifiers[j], win, False, BUTTONMASK, GrabModeAsync, GrabModeSync, None, None);
//             }
//         }
//     }
// }


/*
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
*/

/*

// this sets the border to a specific color
XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColBorder].pixel);

*/
/*
static void showhide(Client *c) {
	if (!c) { return; }
	// show clients top down
	if (ISVISIBLE(c)) {
		XMoveWindow(dpy, c->win, c->x, c->y);
		if ((!c->mon->lt[c->mon->sellt]->arrange || c->isfloating) && !c->isfullscreen) { resize(c, c->x, c->y, c->w, c->h, 0); }
		showhide(c->snext);
	} else { // hide clients bottom up
		showhide(c->snext);
	}
}
*/
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
