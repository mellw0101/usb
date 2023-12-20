/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/X.h>
#include <cstddef>


static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define ALT Mod1Mask
#define SUPER Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ ALT,                       KEY,      	view,           {.ui = 1 << TAG} }, \
	{ ALT|ControlMask,           KEY,      	toggleview,     {.ui = 1 << TAG} }, \
	{ ALT|ShiftMask,             KEY,      	tag,            {.ui = 1 << TAG} }, \
	{ ALT|ControlMask|ShiftMask, KEY,      	toggletag,      {.ui = 1 << TAG} },

#define SHIFTVIEW(KEY,TAG) \
	{ SUPER|ControlMask,			KEY,		shiftview,		{.i = TAG} }, \
	{ SUPER|ShiftMask|ControlMask,	KEY,		shiftview_app,	{.i = TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/bash", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
	/* modifier                  key		function        	argument */
	{ ALT,                       XK_p,		spawn,          	{.v = dmenucmd } },
	{ ALT|ShiftMask,             XK_Return,	spawn,          	{.v = termcmd } },
	{ ALT,                       XK_b,		togglebar,      	{0} },
	{ ALT,                       XK_j,		focusstack,     	{.i = +1 } },
	{ ALT,                       XK_k,		focusstack,	     	{.i = -1 } },
	{ ALT,                       XK_i,      incnmaster,			{.i = +1 } },
	{ ALT,                       XK_d,      incnmaster,	     	{.i = -1 } },
	{ ALT,                       XK_h,      setmfact,	       	{.f = -0.05} },
	{ ALT,                       XK_l,      setmfact,	      	{.f = +0.05} },
	{ ALT,                       XK_Return, zoom,	           	{0} },
	{ ALT,                       XK_Tab,    view,           	{0} },
	{ ALT|ShiftMask,             XK_c,      killclient,   		{0} },
	{ ALT,                       XK_t,		setlayout,      	{.v = &layouts[0]} },
	{ ALT,                       XK_f,		setlayout,      	{.v = &layouts[1]} },
	{ ALT,                       XK_m,		setlayout,      	{.v = &layouts[2]} },
	{ ALT,                       XK_space,	setlayout,      	{0} },
	{ ALT|ShiftMask,             XK_space,	togglefloating,		{0} },
	// { ALT,                       XK_0,		view,				{.ui = ~0 } },
	// { ALT|ShiftMask,             XK_0,		tag,				{.ui = ~0 } },
	{ ALT,                       XK_comma,	focusmon,			{.i = -1 } },
	{ ALT,                       XK_period,	focusmon,			{.i = +1 } },
	{ ALT|ShiftMask,             XK_comma,	tagmon,				{.i = -1 } },
	{ ALT|ShiftMask,             XK_period,	tagmon,     	    {.i = +1 } },
	TAGKEYS(                     XK_1,							0)
	TAGKEYS(                     XK_2,							1)
	TAGKEYS(                     XK_3,							2)
	TAGKEYS(                     XK_4,							3)
	TAGKEYS(                     XK_5,							4)
	TAGKEYS(                     XK_6,							5)
	TAGKEYS(                     XK_7,							6)
	TAGKEYS(                     XK_8,							7)
	TAGKEYS(                     XK_9,							8)
	// { MODKEY|ShiftMask,             XK_q,		quit,	           	{0} },

	// my bindings
	{ ALT|ControlMask,			XK_t,		spawn,				SHCMD("konsole") },
	{ ALT|ShiftMask,			XK_q,		spawn,				SHCMD("dwm-KILL") },
	// { ALT|ShiftMask,			XK_q,		quit,				{0} },

	SHIFTVIEW(						XK_Left,						-1)
	SHIFTVIEW(						XK_Right,						+1)

	// { SUPER|ControlMask,			XK_Left,	shiftview, 			{.i = -1} },
	// { SUPER|ControlMask,			XK_Right,	shiftview,		 	{.i = 1} },
	// { SUPER|ControlMask|ShiftMask,	XK_Left,	shiftview_app,		{.i = -1} },
	// { SUPER|ControlMask|ShiftMask,	XK_Right,	shiftview_app,	 	{.i = 1} },
	// { Mod4Mask|ControlMask, 		XK_Left,	viewprevtag,		{0} },
	// { Mod4Mask|ControlMask, 		XK_Right, 	viewnexttag, 		{0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         ALT,       	  	Button1,        movemouse,      {0} },
	{ ClkClientWin,         ALT,         	Button2,        togglefloating, {0} },
	{ ClkClientWin,         ALT,         	Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            ALT,         	Button1,        tag,            {0} },
	{ ClkTagBar,            ALT,         	Button3,        toggletag,      {0} },
};

