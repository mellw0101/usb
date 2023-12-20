// /* See LICENSE file for copyright and license details. */
//
// #include <stdarg.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//
//
// #define MAX(A, B)               ((A) > (B) ? (A) : (B))
// #define MIN(A, B)               ((A) < (B) ? (A) : (B))
// #define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))
//
// void die(const char *fmt, ...);
// void *ecalloc(size_t nmemb, size_t size);

#pragma once
#include <cstdarg>
#include <cstring>  // Include for memset
#include <iostream>
#include <memory>
#include <stdexcept>

#define Min(A, B)               ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))

template <typename T>
T MAX(T a, T b) {
    return (a > b) ? a : b;
}

template <typename T>
T MIN(T a, T b) {
    return (a < b) ? a : b;
}

template <typename T>
bool between(T x, T a, T b) {
    return a <= x && x <= b;
}

[[noreturn]] void die(const std::string &msg);
std::unique_ptr<char[]> Uecalloc(size_t nmemb, size_t size);
void *ecalloc(size_t nmemb, size_t size);


// void
// manage(Window w, XWindowAttributes *wa)
// {
// 	std::unique_ptr<Client> c = std::make_unique<Client>();
//     Client *t = nullptr;
//     Window trans = None;
//     XWindowChanges wc;
//
// 	c->win = w;
// 	/* geometry */
// 	c->x = c->oldx = wa->x;
// 	c->y = c->oldy = wa->y;
// 	c->w = c->oldw = wa->width;
// 	c->h = c->oldh = wa->height;
// 	c->oldbw = wa->border_width;
//
// 	updatetitle(c.get());                                           // Use c.get() to pass the raw pointer
//     if (XGetTransientForHint(dpy, w, &trans) && (t = wintoclient(trans))) {
//         c->mon = t->mon;
//         c->tags = t->tags;
//     } else {
//         c->mon = selmon;
//         applyrules(c.get());
//     }
//
// 	if (c->x + WIDTH(c) > c->mon->wx + c->mon->ww)
// 		c->x = c->mon->wx + c->mon->ww - WIDTH(c);
// 	if (c->y + HEIGHT(c) > c->mon->wy + c->mon->wh)
// 		c->y = c->mon->wy + c->mon->wh - HEIGHT(c);
// 	c->x = MAX(c->x, c->mon->wx);
// 	c->y = MAX(c->y, c->mon->wy);
// 	c->bw = borderpx;
//
// 	wc.border_width = c->bw;
// 	XConfigureWindow(dpy, w, CWBorderWidth, &wc);
// 	XSetWindowBorder(dpy, w, scheme[SchemeNorm][ColBorder].pixel);
// 	configure(c.get()); /* propagates border_width, if size doesn't change */
// 	updatewindowtype(c.get());
// 	updatesizehints(c.get());
// 	updatewmhints(c.get());
// 	XSelectInput(dpy, w, EnterWindowMask|FocusChangeMask|PropertyChangeMask|StructureNotifyMask);
// 	grabbuttons(c.get(), 0);
// 	if (!c->isfloating)
// 		c->isfloating = c->oldstate = trans != None || c->isfixed;
// 	if (c->isfloating)
// 		XRaiseWindow(dpy, c->win);
// 	attach(c.get());
//     attachstack(c.get());
// 	XChangeProperty(dpy, root, netatom[NetClientList], XA_WINDOW, 32, PropModeAppend,
// 		(unsigned char *) &(c->win), 1);
// 	XMoveResizeWindow(dpy, c->win, c->x + 2 * sw, c->y, c->w, c->h); /* some windows require this */
// 	setclientstate(c.get(), NormalState);
// 	if (c->mon == selmon)
// 		unfocus(selmon->sel, 0);
// 	c->mon->sel = c.get();
// 	arrange(c->mon);
// 	XMapWindow(dpy, c->win);
// 	focus(NULL);
// }
