#include "buttonpress.h"

void focusMonitor(XButtonPressedEvent *ev) {
    Monitor *m;
    if ((m = wintomon(ev->window)) && m != selmon) {
        unfocus(selmon->sel, 1);
        selmon = m;
        focus(nullptr);
    }
}

unsigned int processBarClick(XButtonPressedEvent *ev) {
    unsigned int i, x, click = ClkRootWin;
    i = x = 0;
    do
        x += TEXTW(tags[i]);
    while (ev->x >= x && ++i < LENGTH(tags));

    if (i < LENGTH(tags)) {
        click = ClkTagBar;
        arg.ui = 1 << i;
    }
    else if (ev->x < x + TEXTW(selmon->ltsymbol))
        click = ClkLtSymbol;
    else if (ev->x > selmon->ww - (int)TEXTW(stext))
        click = ClkStatusText;
    else
        click = ClkWinTitle;

    return click;
}


unsigned int handleClientWindowClick(XButtonPressedEvent *ev) {
    Client *c;
    unsigned int click = ClkRootWin;
    if ((c = wintoclient(ev->window))) {
        focus(c);
        restack(selmon);
        XAllowEvents(dpy, ReplayPointer, CurrentTime);
        click = ClkClientWin;
    }
    return click;
}


void executeButtonFunction(unsigned int click, XButtonPressedEvent *ev) {
    Arg arg = {0};
    for (unsigned int i = 0; i < LENGTH(buttons); i++) {
        if (click == buttons[i].click && buttons[i].func &&
            buttons[i].button == ev->button &&
            CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state))
            buttons[i].func(
                click == ClkTagBar && buttons[i].arg.i == 0 ? &arg : &buttons[i].arg);
    }
}

void buttonpress(XEvent *e) {
    XButtonPressedEvent *ev = &e->xbutton;
    unsigned int click = ClkRootWin;

    // Focus monitor if necessary
    focusMonitor(ev);

    // Handle bar and client window clicks
    if (ev->window == selmon->barwin)
        click = processBarClick(ev);
    else
        click = handleClientWindowClick(ev);

    // Execute the corresponding button function
    executeButtonFunction(click, ev);
}

