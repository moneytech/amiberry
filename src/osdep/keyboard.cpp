#include "sysconfig.h"
#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "uae.h"
#include "include/memory.h"
#include "newcpu.h"
#include "custom.h"
#include "xwin.h"
#include "drawing.h"
#include "inputdevice.h"
#include "keyboard.h"
#include "keybuf.h"
#include "gui.h"
#include <SDL.h>

char keyboard_type = 0;

static struct uae_input_device_kbr_default keytrans_amiga_x11[] = {

{ 9, INPUTEVENT_KEY_ESC },
{ 67, INPUTEVENT_KEY_F1 },
{ 68, INPUTEVENT_KEY_F2 },
{ 69, INPUTEVENT_KEY_F3 },
{ 70, INPUTEVENT_KEY_F4 },
{ 71, INPUTEVENT_KEY_F5 },
{ 72, INPUTEVENT_KEY_F6 },
{ 73, INPUTEVENT_KEY_F7 },
{ 74, INPUTEVENT_KEY_F8 },
{ 75, INPUTEVENT_KEY_F9 },
{ 76, INPUTEVENT_KEY_F10 },
//{ 95,   INPUTEVENT_KEY_F11},
//{ 96,   INPUTEVENT_KEY_F12},

{ 49, INPUTEVENT_KEY_BACKQUOTE },

{ 10, INPUTEVENT_KEY_1 },
{ 11, INPUTEVENT_KEY_2 },
{ 12, INPUTEVENT_KEY_3 },
{ 13, INPUTEVENT_KEY_4 },
{ 14, INPUTEVENT_KEY_5 },
{ 15, INPUTEVENT_KEY_6 },
{ 16, INPUTEVENT_KEY_7 },
{ 17, INPUTEVENT_KEY_8 },
{ 18, INPUTEVENT_KEY_9 },
{ 19, INPUTEVENT_KEY_0 },
{ 20, INPUTEVENT_KEY_SUB },
{ 21, INPUTEVENT_KEY_EQUALS },
{ 22, INPUTEVENT_KEY_BACKSPACE },

{ 23, INPUTEVENT_KEY_TAB },
{ 24, INPUTEVENT_KEY_Q },
{ 25, INPUTEVENT_KEY_W },
{ 26, INPUTEVENT_KEY_E },
{ 27, INPUTEVENT_KEY_R },
{ 28, INPUTEVENT_KEY_T },
{ 29, INPUTEVENT_KEY_Y },
{ 30, INPUTEVENT_KEY_U },
{ 31, INPUTEVENT_KEY_I },
{ 32, INPUTEVENT_KEY_O },
{ 33, INPUTEVENT_KEY_P },
{ 34, INPUTEVENT_KEY_LEFTBRACKET },
{ 35, INPUTEVENT_KEY_RIGHTBRACKET },
{ 36, INPUTEVENT_KEY_RETURN },

{ 66, INPUTEVENT_KEY_CAPS_LOCK },
{ 38, INPUTEVENT_KEY_A },
{ 39, INPUTEVENT_KEY_S },
{ 40, INPUTEVENT_KEY_D },
{ 41, INPUTEVENT_KEY_F },
{ 42, INPUTEVENT_KEY_G },
{ 43, INPUTEVENT_KEY_H },
{ 44, INPUTEVENT_KEY_J },
{ 45, INPUTEVENT_KEY_K },
{ 46, INPUTEVENT_KEY_L },
{ 47, INPUTEVENT_KEY_SEMICOLON },
{ 48, INPUTEVENT_KEY_SINGLEQUOTE },
{ 51, INPUTEVENT_KEY_BACKSLASH },

{ 50, INPUTEVENT_KEY_SHIFT_LEFT },
{ 94, INPUTEVENT_KEY_LTGT },
{ 52, INPUTEVENT_KEY_Z },
{ 53, INPUTEVENT_KEY_X },
{ 54, INPUTEVENT_KEY_C },
{ 55, INPUTEVENT_KEY_V },
{ 56, INPUTEVENT_KEY_B },
{ 57, INPUTEVENT_KEY_N },
{ 58, INPUTEVENT_KEY_M },
{ 59, INPUTEVENT_KEY_COMMA },
{ 60, INPUTEVENT_KEY_PERIOD },
{ 61, INPUTEVENT_KEY_DIV },
{ 62, INPUTEVENT_KEY_SHIFT_RIGHT },

{ 37, INPUTEVENT_KEY_CTRL },
{ 64, INPUTEVENT_KEY_ALT_LEFT },
{ 65, INPUTEVENT_KEY_SPACE },

{ 108, INPUTEVENT_KEY_ALT_RIGHT },

//{ 78,  INPUTEVENT_KEY_SCROLLOCK},

//{ 77,  INPUTEVENT_KEY_NUMLOCK},
{ 106, INPUTEVENT_KEY_NP_DIV },
{ 63, INPUTEVENT_KEY_NP_MUL },
{ 82, INPUTEVENT_KEY_NP_SUB },

{ 79, INPUTEVENT_KEY_NP_7 },
{ 80, INPUTEVENT_KEY_NP_8 },
{ 81, INPUTEVENT_KEY_NP_9 },
{ 86, INPUTEVENT_KEY_NP_ADD },

{ 83, INPUTEVENT_KEY_NP_4 },
{ 84, INPUTEVENT_KEY_NP_5 },
{ 85, INPUTEVENT_KEY_NP_6 },

{ 87, INPUTEVENT_KEY_NP_1 },
{ 88, INPUTEVENT_KEY_NP_2 },
{ 89, INPUTEVENT_KEY_NP_3 },
{ 104, INPUTEVENT_KEY_ENTER },         // The ENT from keypad..

{ 90, INPUTEVENT_KEY_NP_0 },
{ 91, INPUTEVENT_KEY_NP_PERIOD },

{ 111, INPUTEVENT_KEY_CURSOR_UP },
{ 113, INPUTEVENT_KEY_CURSOR_LEFT },
{ 116, INPUTEVENT_KEY_CURSOR_DOWN },
{ 114, INPUTEVENT_KEY_CURSOR_RIGHT },


{ 110, INPUTEVENT_KEY_NP_LPAREN },     // Map home   to left  parent (as fsuae)
{ 112, INPUTEVENT_KEY_NP_RPAREN },     // Map pageup to right parent (as fsuae)

{ 115, INPUTEVENT_KEY_HELP },          // Help mapped to End key (as fsuae)

{ 119, INPUTEVENT_KEY_DEL },

{ 133, INPUTEVENT_KEY_AMIGA_LEFT },   // Left amiga mapped to left Windows
{ 134, INPUTEVENT_KEY_AMIGA_RIGHT },  // Right amiga mapped to right windows key.
{ 135, INPUTEVENT_KEY_AMIGA_RIGHT },  // Right amiga mapped to Menu key.
{ -1, 0 }
};

static struct uae_input_device_kbr_default keytrans_amiga_fbcon[] = {

{ 9 - 8, INPUTEVENT_KEY_ESC },
{ 67 - 8, INPUTEVENT_KEY_F1 },
{ 68 - 8, INPUTEVENT_KEY_F2 },
{ 69 - 8, INPUTEVENT_KEY_F3 },
{ 70 - 8, INPUTEVENT_KEY_F4 },
{ 71 - 8, INPUTEVENT_KEY_F5 },
{ 72 - 8, INPUTEVENT_KEY_F6 },
{ 73 - 8, INPUTEVENT_KEY_F7 },
{ 74 - 8, INPUTEVENT_KEY_F8 },
{ 75 - 8, INPUTEVENT_KEY_F9 },
{ 76 - 8, INPUTEVENT_KEY_F10 },
// { 95 -8 ,   INPUTEVENT_KEY_F11},
// { 96 -8 ,   INPUTEVENT_KEY_F12},

{ 49 - 8, INPUTEVENT_KEY_BACKQUOTE },

{ 10 - 8, INPUTEVENT_KEY_1 },
{ 11 - 8, INPUTEVENT_KEY_2 },
{ 12 - 8, INPUTEVENT_KEY_3 },
{ 13 - 8, INPUTEVENT_KEY_4 },
{ 14 - 8, INPUTEVENT_KEY_5 },
{ 15 - 8, INPUTEVENT_KEY_6 },
{ 16 - 8, INPUTEVENT_KEY_7 },
{ 17 - 8, INPUTEVENT_KEY_8 },
{ 18 - 8, INPUTEVENT_KEY_9 },
{ 19 - 8, INPUTEVENT_KEY_0 },
{ 20 - 8, INPUTEVENT_KEY_SUB },
{ 21 - 8, INPUTEVENT_KEY_EQUALS },
{ 22 - 8, INPUTEVENT_KEY_BACKSPACE },

{ 23 - 8, INPUTEVENT_KEY_TAB },
{ 24 - 8, INPUTEVENT_KEY_Q },
{ 25 - 8, INPUTEVENT_KEY_W },
{ 26 - 8, INPUTEVENT_KEY_E },
{ 27 - 8, INPUTEVENT_KEY_R },
{ 28 - 8, INPUTEVENT_KEY_T },
{ 29 - 8, INPUTEVENT_KEY_Y },
{ 30 - 8, INPUTEVENT_KEY_U },
{ 31 - 8, INPUTEVENT_KEY_I },
{ 32 - 8, INPUTEVENT_KEY_O },
{ 33 - 8, INPUTEVENT_KEY_P },
{ 34 - 8, INPUTEVENT_KEY_LEFTBRACKET },
{ 35 - 8, INPUTEVENT_KEY_RIGHTBRACKET },
{ 36 - 8, INPUTEVENT_KEY_RETURN },

{ 66 - 8, INPUTEVENT_KEY_CAPS_LOCK },
{ 38 - 8, INPUTEVENT_KEY_A },
{ 39 - 8, INPUTEVENT_KEY_S },
{ 40 - 8, INPUTEVENT_KEY_D },
{ 41 - 8, INPUTEVENT_KEY_F },
{ 42 - 8, INPUTEVENT_KEY_G },
{ 43 - 8, INPUTEVENT_KEY_H },
{ 44 - 8, INPUTEVENT_KEY_J },
{ 45 - 8, INPUTEVENT_KEY_K },
{ 46 - 8, INPUTEVENT_KEY_L },
{ 47 - 8, INPUTEVENT_KEY_SEMICOLON },
{ 48 - 8, INPUTEVENT_KEY_SINGLEQUOTE },
{ 51 - 8, INPUTEVENT_KEY_BACKSLASH },

{ 50 - 8, INPUTEVENT_KEY_SHIFT_LEFT },
{ 94 - 8, INPUTEVENT_KEY_LTGT },
{ 52 - 8, INPUTEVENT_KEY_Z },
{ 53 - 8, INPUTEVENT_KEY_X },
{ 54 - 8, INPUTEVENT_KEY_C },
{ 55 - 8, INPUTEVENT_KEY_V },
{ 56 - 8, INPUTEVENT_KEY_B },
{ 57 - 8, INPUTEVENT_KEY_N },
{ 58 - 8, INPUTEVENT_KEY_M },
{ 59 - 8, INPUTEVENT_KEY_COMMA },
{ 60 - 8, INPUTEVENT_KEY_PERIOD },
{ 61 - 8, INPUTEVENT_KEY_DIV },
{ 62 - 8, INPUTEVENT_KEY_SHIFT_RIGHT },

{ 37 - 8, INPUTEVENT_KEY_CTRL },
{ 64 - 8, INPUTEVENT_KEY_ALT_LEFT },
{ 65 - 8, INPUTEVENT_KEY_SPACE },

{ 108 - 8, INPUTEVENT_KEY_ALT_RIGHT },

//{ 78 -8 ,  INPUTEVENT_KEY_SCROLLOCK},

//{ 77 -8 ,  INPUTEVENT_KEY_NUMLOCK},
{ 106 - 8, INPUTEVENT_KEY_NP_DIV },
{ 63 - 8, INPUTEVENT_KEY_NP_MUL },
{ 82 - 8, INPUTEVENT_KEY_NP_SUB },

{ 79 - 8, INPUTEVENT_KEY_NP_7 },
{ 80 - 8, INPUTEVENT_KEY_NP_8 },
{ 81 - 8, INPUTEVENT_KEY_NP_9 },
{ 86 - 8, INPUTEVENT_KEY_NP_ADD },

{ 83 - 8, INPUTEVENT_KEY_NP_4 },
{ 84 - 8, INPUTEVENT_KEY_NP_5 },
{ 85 - 8, INPUTEVENT_KEY_NP_6 },

{ 87 - 8, INPUTEVENT_KEY_NP_1 },
{ 88 - 8, INPUTEVENT_KEY_NP_2 },
{ 89 - 8, INPUTEVENT_KEY_NP_3 },
{ 104 - 8, INPUTEVENT_KEY_ENTER },         // The ENT from keypad..

{ 90 - 8, INPUTEVENT_KEY_NP_0 },
{ 91 - 8, INPUTEVENT_KEY_PERIOD },

{ 111 - 8, INPUTEVENT_KEY_CURSOR_UP },
{ 113 - 8, INPUTEVENT_KEY_CURSOR_LEFT },
{ 116 - 8, INPUTEVENT_KEY_CURSOR_DOWN },
{ 114 - 8, INPUTEVENT_KEY_CURSOR_RIGHT },


{ 110 - 8, INPUTEVENT_KEY_NP_LPAREN },     // Map home   to left  parent (as fsuae)
{ 112 - 8, INPUTEVENT_KEY_NP_RPAREN },     // Map pageup to right parent (as fsuae)

{ 115 - 8, INPUTEVENT_KEY_HELP },          // Help mapped to End key (as fsuae)

{ 119 - 8, INPUTEVENT_KEY_DEL },

{ 133 - 8, INPUTEVENT_KEY_AMIGA_LEFT },   // Left amiga mapped to left Windows
{ 134 - 8, INPUTEVENT_KEY_AMIGA_RIGHT },  // Right amiga mapped to right windows key.
{ 135 - 8, INPUTEVENT_KEY_AMIGA_RIGHT },  // Right amiga mapped to Menu key.
{ -1, 0 }
};

static struct uae_input_device_kbr_default keytrans_amiga[] = {
	
	{ VK_ESCAPE, INPUTEVENT_KEY_ESC },

	{ SDLK_F1, INPUTEVENT_KEY_F1 },
	{ SDLK_F2, INPUTEVENT_KEY_F2 },
	{ SDLK_F3, INPUTEVENT_KEY_F3 },
	{ SDLK_F4, INPUTEVENT_KEY_F4 },
	
	{ SDLK_F5, INPUTEVENT_KEY_F5 },
	{ SDLK_F6, INPUTEVENT_KEY_F6 },
	{ SDLK_F7, INPUTEVENT_KEY_F7 },
	{ SDLK_F8, INPUTEVENT_KEY_F8 },
	{ SDLK_F9, INPUTEVENT_KEY_F9 },
	{ SDLK_F10, INPUTEVENT_KEY_F10 },

	{ SDLK_1, INPUTEVENT_KEY_1 },
	{ SDLK_2, INPUTEVENT_KEY_2 },
	{ SDLK_3, INPUTEVENT_KEY_3 },
	{ SDLK_4, INPUTEVENT_KEY_4 },
	{ SDLK_5, INPUTEVENT_KEY_5 },
	{ SDLK_6, INPUTEVENT_KEY_6 },
	{ SDLK_7, INPUTEVENT_KEY_7 },
	{ SDLK_8, INPUTEVENT_KEY_8 },
	{ SDLK_9, INPUTEVENT_KEY_9 },
	{ SDLK_0, INPUTEVENT_KEY_0 },

	{ SDLK_TAB, INPUTEVENT_KEY_TAB },

	{ SDLK_a, INPUTEVENT_KEY_A },
	{ SDLK_b, INPUTEVENT_KEY_B },
	{ SDLK_c, INPUTEVENT_KEY_C },
	{ SDLK_d, INPUTEVENT_KEY_D },
	{ SDLK_e, INPUTEVENT_KEY_E },
	{ SDLK_f, INPUTEVENT_KEY_F },
	{ SDLK_g, INPUTEVENT_KEY_G },
	{ SDLK_h, INPUTEVENT_KEY_H },
	{ SDLK_i, INPUTEVENT_KEY_I },
	{ SDLK_j, INPUTEVENT_KEY_J },
	{ SDLK_k, INPUTEVENT_KEY_K },
	{ SDLK_l, INPUTEVENT_KEY_L },
	{ SDLK_m, INPUTEVENT_KEY_M },
	{ SDLK_n, INPUTEVENT_KEY_N },
	{ SDLK_o, INPUTEVENT_KEY_O },
	{ SDLK_p, INPUTEVENT_KEY_P },
	{ SDLK_q, INPUTEVENT_KEY_Q },
	{ SDLK_r, INPUTEVENT_KEY_R },
	{ SDLK_s, INPUTEVENT_KEY_S },
	{ SDLK_t, INPUTEVENT_KEY_T },
	{ SDLK_u, INPUTEVENT_KEY_U },
	{ SDLK_v, INPUTEVENT_KEY_V },
	{ SDLK_w, INPUTEVENT_KEY_W },
	{ SDLK_x, INPUTEVENT_KEY_X },
	{ SDLK_y, INPUTEVENT_KEY_Y },
	{ SDLK_z, INPUTEVENT_KEY_Z },

	{ SDLK_CAPSLOCK, INPUTEVENT_KEY_CAPS_LOCK, ID_FLAG_TOGGLE },

#ifdef USE_SDL1
	{ SDLK_KP1, INPUTEVENT_KEY_NP_1 },
	{ SDLK_KP2, INPUTEVENT_KEY_NP_2 },
	{ SDLK_KP3, INPUTEVENT_KEY_NP_3 },
	{ SDLK_KP4, INPUTEVENT_KEY_NP_4 },
	{ SDLK_KP5, INPUTEVENT_KEY_NP_5 },
	{ SDLK_KP6, INPUTEVENT_KEY_NP_6 },
	{ SDLK_KP7, INPUTEVENT_KEY_NP_7 },
	{ SDLK_KP8, INPUTEVENT_KEY_NP_8 },
	{ SDLK_KP9, INPUTEVENT_KEY_NP_9 },
	{ SDLK_KP0, INPUTEVENT_KEY_NP_0 },
#elif USE_SDL2
	{ SDLK_KP_1, INPUTEVENT_KEY_NP_1 },
	{ SDLK_KP_2, INPUTEVENT_KEY_NP_2 },
	{ SDLK_KP_3, INPUTEVENT_KEY_NP_3 },
	{ SDLK_KP_4, INPUTEVENT_KEY_NP_4 },
	{ SDLK_KP_5, INPUTEVENT_KEY_NP_5 },
	{ SDLK_KP_6, INPUTEVENT_KEY_NP_6 },
	{ SDLK_KP_7, INPUTEVENT_KEY_NP_7 },
	{ SDLK_KP_8, INPUTEVENT_KEY_NP_8 },
	{ SDLK_KP_9, INPUTEVENT_KEY_NP_9 },
	{ SDLK_KP_0, INPUTEVENT_KEY_NP_0 },
#endif
	{ SDLK_KP_PERIOD, INPUTEVENT_KEY_PERIOD },
	{ SDLK_KP_PLUS, INPUTEVENT_KEY_NP_ADD },
	{ SDLK_KP_MINUS, INPUTEVENT_KEY_NP_SUB },
	{ SDLK_KP_MULTIPLY, INPUTEVENT_KEY_NP_MUL },
	{ SDLK_KP_DIVIDE, INPUTEVENT_KEY_NP_DIV },
	{ SDLK_KP_ENTER, INPUTEVENT_KEY_ENTER },         // The ENT from keypad..

	{ SDLK_MINUS, INPUTEVENT_KEY_SUB },
	{ SDLK_EQUALS, INPUTEVENT_KEY_EQUALS },
	{ SDLK_BACKSPACE, INPUTEVENT_KEY_BACKSPACE },	
	{ SDLK_RETURN, INPUTEVENT_KEY_RETURN },
	{ SDLK_SPACE, INPUTEVENT_KEY_SPACE },

	{ SDLK_LSHIFT, INPUTEVENT_KEY_SHIFT_LEFT, 0, INPUTEVENT_SPC_QUALIFIER_SHIFT },
	{ SDLK_LCTRL, INPUTEVENT_KEY_CTRL, 0, INPUTEVENT_SPC_QUALIFIER_CONTROL },
#ifdef USE_SDL1
	{ SDLK_LSUPER, INPUTEVENT_KEY_AMIGA_LEFT, 0, INPUTEVENT_SPC_QUALIFIER_WIN },
#elif USE_SDL2
	{ SDLK_LGUI, INPUTEVENT_KEY_AMIGA_LEFT, 0, INPUTEVENT_SPC_QUALIFIER_WIN },
#endif
	{ SDLK_LALT, INPUTEVENT_KEY_ALT_LEFT, 0, INPUTEVENT_SPC_QUALIFIER_ALT },
	{ SDLK_RALT, INPUTEVENT_KEY_ALT_RIGHT, 0, INPUTEVENT_SPC_QUALIFIER_ALT },
#ifdef USE_SDL1
	{ SDLK_RSUPER, INPUTEVENT_KEY_AMIGA_RIGHT, 0, INPUTEVENT_SPC_QUALIFIER_WIN },
#elif USE_SDL2
	{ SDLK_RGUI, INPUTEVENT_KEY_AMIGA_RIGHT, 0, INPUTEVENT_SPC_QUALIFIER_WIN },
#endif
	{ SDLK_MENU, INPUTEVENT_KEY_AMIGA_RIGHT, 0, INPUTEVENT_SPC_QUALIFIER_WIN },
	{ SDLK_RCTRL, INPUTEVENT_KEY_CTRL, 0, INPUTEVENT_SPC_QUALIFIER_CONTROL },
	{ SDLK_RSHIFT, INPUTEVENT_KEY_SHIFT_RIGHT, 0, INPUTEVENT_SPC_QUALIFIER_SHIFT },

	{ SDLK_UP, INPUTEVENT_KEY_CURSOR_UP },
	{ SDLK_DOWN, INPUTEVENT_KEY_CURSOR_DOWN },
	{ SDLK_LEFT, INPUTEVENT_KEY_CURSOR_LEFT },
	{ SDLK_RIGHT, INPUTEVENT_KEY_CURSOR_RIGHT },

	{ SDLK_INSERT, INPUTEVENT_KEY_AMIGA_LEFT },
	{ SDLK_DELETE, INPUTEVENT_KEY_DEL },
	{ SDLK_HOME, INPUTEVENT_KEY_AMIGA_RIGHT },
	{ SDLK_PAGEDOWN, INPUTEVENT_KEY_HELP },
	{ SDLK_PAGEUP, INPUTEVENT_SPC_FREEZEBUTTON },
	
	{ SDLK_LEFTBRACKET, INPUTEVENT_KEY_LEFTBRACKET },
	{ SDLK_RIGHTBRACKET, INPUTEVENT_KEY_RIGHTBRACKET },
	{ SDLK_SEMICOLON, INPUTEVENT_KEY_SEMICOLON },
	{ SDLK_QUOTE, INPUTEVENT_KEY_SINGLEQUOTE },
	{ SDLK_BACKQUOTE, INPUTEVENT_KEY_BACKQUOTE },
	{ SDLK_BACKSLASH, INPUTEVENT_KEY_BACKSLASH },
	{ SDLK_COMMA, INPUTEVENT_KEY_COMMA },	
	{ SDLK_PERIOD, INPUTEVENT_KEY_PERIOD },
	{ SDLK_SLASH, INPUTEVENT_KEY_DIV },

	{SDLK_END, INPUTEVENT_SPC_QUALIFIER_SPECIAL },
	{SDLK_PAUSE, INPUTEVENT_SPC_PAUSE },

	{SDLK_F12, INPUTEVENT_SPC_ENTERGUI },

	{ -1, 0 }
};

static struct uae_input_device_kbr_default *keytrans[] =
{
    keytrans_amiga,
	keytrans_amiga,
	keytrans_amiga
};

static struct uae_input_device_kbr_default *keytrans_x11[] = {
	keytrans_amiga_x11,
	keytrans_amiga_x11,
	keytrans_amiga_x11
};

static struct uae_input_device_kbr_default *keytrans_fbcon[] = {
	keytrans_amiga_fbcon,
	keytrans_amiga_fbcon,
	keytrans_amiga_fbcon
};

#ifdef USE_SDL1
static int kb_np[] = { SDLK_KP4, -1, SDLK_KP6, -1, SDLK_KP8, -1, SDLK_KP2, -1, SDLK_KP0, SDLK_KP5, -1, SDLK_KP_PERIOD, -1, SDLK_KP_ENTER, -1, -1 };
#elif USE_SDL2
static int kb_np[] = { SDLK_KP_4, -1, SDLK_KP_6, -1, SDLK_KP_8, -1, SDLK_KP_2, -1, SDLK_KP_0, SDLK_KP_5, -1, SDLK_KP_DECIMAL, -1, SDLK_KP_ENTER, -1, -1 };
#endif
static int kb_ck[] = { SDLK_LEFT, -1, SDLK_RIGHT, -1, SDLK_UP, -1, SDLK_DOWN, -1, SDLK_RCTRL, SDLK_RALT, -1, SDLK_RSHIFT, -1, -1 };
static int kb_se[] = { SDLK_a, -1, SDLK_d, -1, SDLK_w, -1, SDLK_s, -1, SDLK_LALT, -1, SDLK_LSHIFT, -1, -1 };
#ifdef USE_SDL1
static int kb_np3[] = { SDLK_KP4, -1, SDLK_KP6, -1, SDLK_KP8, -1, SDLK_KP2, -1, SDLK_KP0, SDLK_KP5, -1, SDLK_KP_PERIOD, -1, SDLK_KP_ENTER, -1, -1 };
#elif USE_SDL2
static int kb_np3[] = { SDLK_KP_4, -1, SDLK_KP_6, -1, SDLK_KP_8, -1, SDLK_KP_2, -1, SDLK_KP_0, SDLK_KP_5, -1, SDLK_KP_DECIMAL, -1, SDLK_KP_ENTER, -1, -1 };
#endif
static int kb_ck3[] = { SDLK_LEFT, -1, SDLK_RIGHT, -1, SDLK_UP, -1, SDLK_DOWN, -1, SDLK_RCTRL, -1, SDLK_RSHIFT, -1, SDLK_RALT, -1, -1 };
static int kb_se3[] = { SDLK_a, -1, SDLK_d, -1, SDLK_w, -1, SDLK_s, -1, SDLK_LALT, -1, SDLK_LSHIFT, -1, SDLK_LCTRL, -1, -1 };

#ifdef USE_SDL1
static int kb_cd32_np[] = { SDLK_KP4, -1, SDLK_KP6, -1, SDLK_KP8, -1, SDLK_KP2, -1, SDLK_KP0, SDLK_KP5, SDLK_KP1, -1, SDLK_KP_PERIOD, SDLK_KP3, -1, SDLK_KP7, -1, SDLK_KP9, -1, SDLK_KP_DIVIDE, -1, SDLK_KP_MINUS, -1, SDLK_KP_MULTIPLY, -1, -1 };
#elif USE_SDL2
static int kb_cd32_np[] = { SDLK_KP_4, -1, SDLK_KP_6, -1, SDLK_KP_8, -1, SDLK_KP_2, -1, SDLK_KP_0, SDLK_KP_5, SDLK_KP_1, -1, SDLK_KP_DECIMAL, SDLK_KP_3, -1, SDLK_KP_7, -1, SDLK_KP_9, -1, SDLK_KP_DIVIDE, -1, SDLK_KP_MINUS, -1, SDLK_KP_MULTIPLY, -1, -1 };
#endif

#ifdef USE_SDL1
static int kb_cd32_ck[] = { SDLK_LEFT, -1, SDLK_RIGHT, -1, SDLK_UP, -1, SDLK_DOWN, -1, SDLK_RCTRL, -1, SDLK_RALT, -1, SDLK_KP7, -1, SDLK_KP9, -1, SDLK_KP_DIVIDE, -1, SDLK_KP_MINUS, -1, SDLK_KP_MULTIPLY, -1, -1 };
static int kb_cd32_se[] = { SDLK_a, -1, SDLK_d, -1, SDLK_w, -1, SDLK_s, -1, -1, SDLK_LALT, -1, SDLK_LSHIFT, -1, SDLK_KP7, -1, SDLK_KP9, -1, SDLK_KP_DIVIDE, -1, SDLK_KP_MINUS, -1, SDLK_KP_MULTIPLY, -1, -1 };
#elif USE_SDL2
static int kb_cd32_ck[] = { SDLK_LEFT, -1, SDLK_RIGHT, -1, SDLK_UP, -1, SDLK_DOWN, -1, SDLK_RCTRL, -1, SDLK_RALT, -1, SDLK_KP_7, -1, SDLK_KP_9, -1, SDLK_KP_DIVIDE, -1, SDLK_KP_MINUS, -1, SDLK_KP_MULTIPLY, -1, -1 };
static int kb_cd32_se[] = { SDLK_a, -1, SDLK_d, -1, SDLK_w, -1, SDLK_s, -1, -1, SDLK_LALT, -1, SDLK_LSHIFT, -1, SDLK_KP_7, -1, SDLK_KP_9, -1, SDLK_KP_DIVIDE, -1, SDLK_KP_MINUS, -1, SDLK_KP_MULTIPLY, -1, -1 };
#endif

#ifdef USE_SDL1
static int kb_cdtv[] = { SDLK_KP1, -1, SDLK_KP3, -1, SDLK_KP7, -1, SDLK_KP9, -1, -1 };

static int kb_xa1[] = { SDLK_KP4, -1, SDLK_KP6, -1, SDLK_KP8, -1, SDLK_KP2, SDLK_KP5, -1, SDLK_LCTRL, -1, SDLK_LALT, -1, SDLK_SPACE, -1, -1 };
#elif USE_SDL2
static int kb_cdtv[] = { SDLK_KP_1, -1, SDLK_KP_3, -1, SDLK_KP_7, -1, SDLK_KP_9, -1, -1 };

static int kb_xa1[] = { SDLK_KP_4, -1, SDLK_KP_6, -1, SDLK_KP_8, -1, SDLK_KP_2, SDLK_KP_5, -1, SDLK_LCTRL, -1, SDLK_LALT, -1, SDLK_SPACE, -1, -1 };
#endif
static int kb_xa2[] = { SDLK_d, -1, SDLK_g, -1, SDLK_r, -1, SDLK_f, -1, SDLK_a, -1, SDLK_s, -1, SDLK_q, -1 };
static int kb_arcadia[] = { SDLK_F2, -1, SDLK_1, -1, SDLK_2, -1, SDLK_5, -1, SDLK_6, -1, -1 };
static int kb_arcadiaxa[] = { SDLK_1, -1, SDLK_2, -1, SDLK_3, -1, SDLK_4, -1, SDLK_6, -1, SDLK_LEFTBRACKET, SDLK_LSHIFT, -1, SDLK_RIGHTBRACKET, -1, SDLK_c, -1, SDLK_5, -1, SDLK_z, -1, SDLK_x, -1, -1 };

static int *kbmaps[] = {
	kb_np, kb_ck, kb_se, kb_np3, kb_ck3, kb_se3,
	kb_cd32_np, kb_cd32_ck, kb_cd32_se,
	kb_xa1, kb_xa2, kb_arcadia, kb_arcadiaxa, kb_cdtv
};

void keyboard_settrans(void)
{
#ifdef USE_SDL1
	char vid_drv_name[32];
	// get display type...
	SDL_VideoDriverName(vid_drv_name, sizeof vid_drv_name);
	if (strcmp(vid_drv_name, "x11") == 0)
	{
		keyboard_type = KEYCODE_X11;
		inputdevice_setkeytranslation(keytrans_x11, kbmaps);
	}
	else  if (strcmp(vid_drv_name, "fbcon") == 0)
	{
		keyboard_type = KEYCODE_FBCON;
		inputdevice_setkeytranslation(keytrans_fbcon, kbmaps);
	}
	else
	{
		keyboard_type = KEYCODE_UNK;
		inputdevice_setkeytranslation(keytrans, kbmaps);
	}
#elif USE_SDL2
	inputdevice_setkeytranslation(keytrans, kbmaps);
#endif
}

static bool specialpressed()
{
	return (input_getqualifiers() & ID_FLAG_QUALIFIER_SPECIAL) != 0;
}
static bool shiftpressed()
{
	return (input_getqualifiers() & ID_FLAG_QUALIFIER_SHIFT) != 0;
}
static bool altpressed()
{
	return (input_getqualifiers() & ID_FLAG_QUALIFIER_ALT) != 0;
}
static bool ctrlpressed()
{
	return (input_getqualifiers() & ID_FLAG_QUALIFIER_CONTROL) != 0;
}

static int capslockstate;
static int host_capslockstate, host_numlockstate, host_scrolllockstate;

int getcapslockstate()
{
	return capslockstate;
}
void setcapslockstate(int state)
{
	capslockstate = state;
}

void clearallkeys()
{
	inputdevice_updateconfig(&changed_prefs, &currprefs);
}

#ifdef USE_SDL1
static const int np[] = {
	SDLK_KP0, 0, SDLK_KP_PERIOD, 0, SDLK_KP1, 1, SDLK_KP2, 2,
	SDLK_KP3, 3, SDLK_KP4, 4, SDLK_KP5, 5, SDLK_KP6, 6, SDLK_KP7, 7,
	SDLK_KP8, 8, SDLK_KP9, 9, -1 };
#elif USE_SDL2
static const int np[] = {
	SDLK_KP_0, 0, SDLK_KP_PERIOD, 0, SDLK_KP_1, 1, SDLK_KP_2, 2,
	SDLK_KP_3, 3, SDLK_KP_4, 4, SDLK_KP_5, 5, SDLK_KP_6, 6, SDLK_KP_7, 7,
	SDLK_KP_8, 8, SDLK_KP_9, 9, -1 };
#endif

int target_checkcapslock(const int scancode, int *state)
{
#ifdef USE_SDL1
	if (scancode != SDLK_CAPSLOCK && scancode != SDLK_NUMLOCK && scancode != SDLK_SCROLLOCK)
		return 0;
	if (*state == 0)
		return -1;
	if (scancode == SDLK_CAPSLOCK)
		*state = host_capslockstate;
	if (scancode == SDLK_NUMLOCK)
		*state = host_numlockstate;
	if (scancode == SDLK_SCROLLOCK)
		*state = host_scrolllockstate;
	return 1;
#elif USE_SDL2
	if (scancode != SDLK_CAPSLOCK && scancode != SDLK_NUMLOCKCLEAR && scancode != SDLK_SCROLLLOCK)
		return 0;
	if (*state == 0)
		return -1;
	if (scancode == SDLK_CAPSLOCK)
		*state = host_capslockstate;
	if (scancode == SDLK_NUMLOCKCLEAR)
		*state = host_numlockstate;
	if (scancode == SDLK_SCROLLLOCK)
		*state = host_scrolllockstate;
	return 1;
#endif
}
