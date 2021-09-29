/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#483d8b";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { " 1 ", " 2 ", " 3 ", " 4 ", " 5 " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "firefox",  NULL,       NULL,       1,            0,           -1 },
	{ "Emacs",    NULL,       NULL,       2,            0,           -1 },
	{ "st-tmux",  NULL,       NULL,       4,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* include */
#include <X11/XF86keysym.h>

/* key definitions */
#define XK_NO_MOD 0
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier          key                        function         argument */
	{ MODKEY,            XK_l,                      spawn,           {.v = dmenucmd } },
	{ MODKEY,            XK_x,                      spawn,           {.v = termcmd } },
	/* focus next client */
	{ MODKEY,            XK_o,                      focusstack,      {.i = +1 } },
	/* focus prev/next tag */
	/* { MODKEY,            XK_p,                      shiftview,      {.i = -1 } }, */
	/* { MODKEY,            XK_n,                      shiftview,      {.i = +1 } }, */
	{ MODKEY,                       XK_p,   viewtoleft,     {0} },
	{ MODKEY,                       XK_n,   viewtoright,    {0} },
	{ MODKEY|ShiftMask,             XK_p,   tagtoleft,      {0} },
	{ MODKEY|ShiftMask,             XK_n,   tagtoright,     {0} },
	/* kill focused client */
	{ MODKEY,            XK_d,                      killclient,      {0} },
	/* change gaps */
	/* { MODKEY,            XK_Down,                   setgaps,         {.i = -5 } }, */
	/* { MODKEY,            XK_Up,                     setgaps,         {.i = +5 } }, */
	/* change windows in master */
	{ MODKEY|ShiftMask,  XK_minus,                  incnmaster,      {.i = -1 } },
	{ MODKEY|ShiftMask,  XK_equal,                  incnmaster,      {.i = +1 } },
	/* toggle layout */
	{ MODKEY,            XK_m,                      setlayout,       {0} },
	/* zoom */
	{ MODKEY|ShiftMask,  XK_o,                      zoom,            {0} },
	/* change mfact */
	{ MODKEY,            XK_Left,                   setmfact,        {.f = -0.05 } },
	{ MODKEY,            XK_Right,                  setmfact,        {.f = +0.05 } },
	/* restart & dmenu system-options script */
	{ MODKEY|ShiftMask,  XK_r,                      quit,            {0} },
	{ MODKEY,            XK_q,                      spawn,           SHCMD("system-options") },
	/* alsa volume controls */
	{ XK_NO_MOD,         XF86XK_AudioLowerVolume,   spawn,           SHCMD("amixer sset Master 1db-") },
	{ XK_NO_MOD,         XF86XK_AudioRaiseVolume,   spawn,           SHCMD("amixer sset Master 1db+") },
	{ XK_NO_MOD,         XF86XK_AudioMute,          spawn,           SHCMD("amixer sset Master toggle") },
	/* x220 keyboard function keys */
	{ XK_NO_MOD,         XF86XK_AudioMicMute,       spawn,           SHCMD("amixer sset Capture toggle") },
	{ XK_NO_MOD,         XF86XK_Launch1,            spawn,           SHCMD("system-options") },
	{ XK_NO_MOD,         XF86XK_ScreenSaver,        spawn,           SHCMD("betterlockscreen -l") },
	{ XK_NO_MOD,         XF86XK_TouchpadToggle,     spawn,           SHCMD("touchpad-toggle") },
	/* mpc media controls */
	{ XK_NO_MOD,         XF86XK_AudioPlay,          spawn,           SHCMD("mpc toggle") },
	{ XK_NO_MOD,         XF86XK_AudioStop,          spawn,           SHCMD("mpc stop") },
	{ XK_NO_MOD,         XF86XK_AudioPrev,          spawn,           SHCMD("mpc prev") },
	{ XK_NO_MOD,         XF86XK_AudioNext,          spawn,           SHCMD("mpc next") },
	/* change backlight */
	{ XK_NO_MOD,         XF86XK_MonBrightnessDown,  spawn,           SHCMD("xbacklight -dec 5") },
	{ XK_NO_MOD,         XF86XK_MonBrightnessUp,    spawn,           SHCMD("xbacklight -inc 5") },
	/* toggle status bar */
	{ MODKEY,            XK_f,                      togglebar,       {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button1,        focusstack,     {.i = +1 } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
};

