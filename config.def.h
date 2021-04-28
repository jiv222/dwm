/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 3, .gappx = 3};
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Siji", "undefined Medium:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_base[]        = "#141119";
static const char col_gray[]        = "#9e9d9c";
static const char col_gray1[]       = "#53505c";
static const char col_yellow[]      = "#ffce7e";
static const char col_cyan[]        = "#88d7c9";
static const char *colors[][3]      = {
	/*               fg          bg         border   */
	[SchemeNorm] = { col_gray,   col_base,  col_gray1 },
	[SchemeSel]  = { col_yellow, col_gray1, col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class				instance    title			tags mask	isfloating	monitor */
	{ "Gimp",				NULL,       NULL,			0,			1,			-1 },
	{ "Arandr",				NULL,       NULL,			0,			1,			-1 },
	{ "Pavucontrol",		NULL,       NULL,			0,			1,			-1 },
	{ "TelegramDesktop",	NULL,       "Media viewer",	1 << 7,		1,			-1 },
	{ "discord",			NULL,       NULL,			1 << 6,		0,			-1 },
	{ "Zathura",			NULL,       NULL,			1 << 4,		0,			-1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define SHIFT ShiftMask
#define CTRL ControlMask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,				KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|CTRL,			KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|SHIFT,			KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|CTRL|SHIFT,	KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "Scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *playpause[] = { "playerctl", "--all-players", "play-pause", NULL };
static const char *nexttrack[] = { "playerctl", "--all-players", "next", NULL };
static const char *prevtrack[] = { "playerctl", "--all-players", "previous", NULL };
static const char *passmenu[] = { "passmenu", NULL };
static const char *telegram[] = { "telegram-desktop", NULL };
static const char *firefox[] = { "firefox", NULL };
static const char *discord[] = { "discord", NULL };
static const char *zathura[] = { "zathura", NULL };
static const char *suspend[] = { "loginctl", "suspend", NULL };
static const char *vpn[] = { "doas", "/home/four/.local/bin/dmenu_wg.sh", NULL };

#include "focusurgent.c"
static Key keys[] = {
	/* modifier			key			function        argument */
	{ MODKEY,			XK_p,		spawn,          {.v = dmenucmd } },
	{ MODKEY|SHIFT,		XK_Return,	spawn,          {.v = termcmd } },
	{ MODKEY,			XK_grave,	togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,			XK_b,		togglebar,      {0} },
	/* master */
	{ MODKEY,			XK_i,		incnmaster,     {.i = +1 } },
	{ MODKEY,			XK_d,		incnmaster,     {.i = -1 } },
	{ MODKEY,			XK_h,		setmfact,       {.f = -0.05} },
	{ MODKEY,			XK_l,		setmfact,       {.f = +0.05} },
	/* cfact */
	{ MODKEY|SHIFT,		XK_h,		setcfact,       {.f = +0.25} },
	{ MODKEY|SHIFT,		XK_l,		setcfact,       {.f = -0.25} },
	{ MODKEY|SHIFT,		XK_o,		setcfact,       {.f =  0.00} },
	/* layouts */
	{ MODKEY,			XK_t,		setlayout,      {.v = &layouts[0]} },
	{ MODKEY,			XK_f,		setlayout,      {.v = &layouts[1]} },
	{ MODKEY,			XK_m,		setlayout,      {.v = &layouts[2]} },
	{ MODKEY,			XK_space,	setlayout,      {0} },
	/* window/tag control */
	{ MODKEY,			XK_Return,	zoom,           {0} },
	{ MODKEY,			XK_Tab,		view,           {0} },
	{ MODKEY,			XK_0,		view,           {.ui = ~0 } },
	{ MODKEY|SHIFT,		XK_0,		tag,            {.ui = ~0 } },
	{ MODKEY|SHIFT,		XK_comma,	tagmon,         {.i = -1 } },
	{ MODKEY|SHIFT,		XK_period,	tagmon,         {.i = +1 } },
	{ MODKEY|SHIFT,		XK_space,	togglefloating, {0} },
	{ MODKEY|SHIFT,		XK_q,		killclient,     {0} },
	/* focus */
	{ MODKEY,			XK_j,		focusstack,     {.i = +1 } },
	{ MODKEY,			XK_k,		focusstack,     {.i = -1 } },
	{ MODKEY,			XK_comma,	focusmon,       {.i = -1 } },
	{ MODKEY,			XK_period,	focusmon,       {.i = +1 } },
	{ MODKEY,			XK_u,		focusurgent,	{0} },
	/* gaps */
	{ MODKEY,			XK_minus,	setgaps,        {.i = -5 } },
	{ MODKEY,			XK_equal,	setgaps,        {.i = +5 } },
	{ MODKEY|SHIFT,		XK_minus,	setgaps,        {.i = GAP_RESET } },
	{ MODKEY|SHIFT,		XK_equal,	setgaps,        {.i = GAP_TOGGLE} },
	{ MODKEY|SHIFT,		XK_z,		spawn,			{.v = suspend } },
	/* backlight control */
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("/usr/bin/light -A 5") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("/usr/bin/light -U 5") },
	/* media control */
	{ 0, XF86XK_AudioNext,	spawn,		{.v = nexttrack } },
	{ 0, XF86XK_AudioPrev,	spawn,		{.v = prevtrack } },
	{ 0, XF86XK_AudioPlay,	spawn,		{.v = playpause } },
	/* dmenu */
	{ MODKEY|CTRL,		XK_p,	spawn,	{.v = passmenu } },
	{ MODKEY|CTRL,		XK_v,	spawn,	{.v = vpn } },
	/* programs */
	{ MODKEY|ALT,		XK_t,	spawn,	{.v = telegram } },
	{ MODKEY|ALT,		XK_f,	spawn,	{.v = firefox } },
	{ MODKEY|ALT,		XK_d,	spawn,	{.v = discord } },
	{ MODKEY|ALT,		XK_z,	spawn,	{.v = zathura } },
	/* tags */
	TAGKEYS( XK_1, 0)
	TAGKEYS( XK_2, 1)
	TAGKEYS( XK_3, 2)
	TAGKEYS( XK_4, 3)
	TAGKEYS( XK_5, 4)
	TAGKEYS( XK_6, 5)
	TAGKEYS( XK_7, 6)
	TAGKEYS( XK_8, 7)
	TAGKEYS( XK_9, 8)
	{ MODKEY|SHIFT,			XK_e,		quit,			{0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
