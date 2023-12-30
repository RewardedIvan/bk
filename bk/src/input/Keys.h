#pragma once

namespace bk::input {
	typedef uint16_t Key;

	struct Keys {
#ifdef BK_INCLUDE_GLFW
		static void SetupGLFW();
#endif
		static inline Key K_SPACE = 0;
		static inline Key K_APOSTROPHE = 0;  /* ' */
		static inline Key K_COMMA = 0;  /* , */
		static inline Key K_MINUS = 0;  /* - */
		static inline Key K_PERIOD = 0;  /* . */
		static inline Key K_SLASH = 0;  /* / */
		static inline Key K_0 = 0;
		static inline Key K_1 = 0;
		static inline Key K_2 = 0;
		static inline Key K_3 = 0;
		static inline Key K_4 = 0;
		static inline Key K_5 = 0;
		static inline Key K_6 = 0;
		static inline Key K_7 = 0;
		static inline Key K_8 = 0;
		static inline Key K_9 = 0;
		static inline Key K_SEMICOLON;  /* ; */
		static inline Key K_EQUAL;  /* = */
		static inline Key K_A = 0;
		static inline Key K_B = 0;
		static inline Key K_C = 0;
		static inline Key K_D = 0;
		static inline Key K_E = 0;
		static inline Key K_F = 0;
		static inline Key K_G = 0;
		static inline Key K_H = 0;
		static inline Key K_I = 0;
		static inline Key K_J = 0;
		static inline Key K_K = 0;
		static inline Key K_L = 0;
		static inline Key K_M = 0;
		static inline Key K_N = 0;
		static inline Key K_O = 0;
		static inline Key K_P = 0;
		static inline Key K_Q = 0;
		static inline Key K_R = 0;
		static inline Key K_S = 0;
		static inline Key K_T = 0;
		static inline Key K_U = 0;
		static inline Key K_V = 0;
		static inline Key K_W = 0;
		static inline Key K_X = 0;
		static inline Key K_Y = 0;
		static inline Key K_Z = 0;
		static inline Key K_LEFT_BRACKET = 0;  /* [ */
		static inline Key K_BACKSLASH = 0;  /* \ */
		static inline Key K_RIGHT_BRACKET = 0;  /* ] */
		static inline Key K_GRAVE_ACCENT = 0;  /* ` */
		//static inline Key K_WORLD_1 = 0; /* non-US #1 */
		//static inline Key K_WORLD_2 = 0; /* non-US #2 */

		/* Function keys */
		static inline Key K_ESCAPE = 0;
		static inline Key K_ENTER = 0;
		static inline Key K_TAB = 0;
		static inline Key K_BACKSPACE = 0;
		static inline Key K_INSERT = 0;
		static inline Key K_DELETE = 0;
		static inline Key K_RIGHT = 0;
		static inline Key K_LEFT = 0;
		static inline Key K_DOWN = 0;
		static inline Key K_UP = 0;
		static inline Key K_PAGE_UP = 0;
		static inline Key K_PAGE_DOWN = 0;
		static inline Key K_HOME = 0;
		static inline Key K_END = 0;
		static inline Key K_CAPS_LOCK = 0;
		static inline Key K_SCROLL_LOCK = 0;
		static inline Key K_NUM_LOCK = 0;
		static inline Key K_PRINT_SCREEN = 0;
		static inline Key K_PAUSE = 0;
		static inline Key K_F1 = 0;
		static inline Key K_F2 = 0;
		static inline Key K_F3 = 0;
		static inline Key K_F4 = 0;
		static inline Key K_F5 = 0;
		static inline Key K_F6 = 0;
		static inline Key K_F7 = 0;
		static inline Key K_F8 = 0;
		static inline Key K_F9 = 0;
		static inline Key K_F10 = 0;
		static inline Key K_F11 = 0;
		static inline Key K_F12 = 0;
		static inline Key K_F13 = 0;
		static inline Key K_F14 = 0;
		static inline Key K_F15 = 0;
		static inline Key K_F16 = 0;
		static inline Key K_F17 = 0;
		static inline Key K_F18 = 0;
		static inline Key K_F19 = 0;
		static inline Key K_F20 = 0;
		static inline Key K_F21 = 0;
		static inline Key K_F22 = 0;
		static inline Key K_F23 = 0;
		static inline Key K_F24 = 0;
		static inline Key K_F25 = 0;
		static inline Key K_KP_0 = 0; // kp = keypad
		static inline Key K_KP_1 = 0;
		static inline Key K_KP_2 = 0;
		static inline Key K_KP_3 = 0;
		static inline Key K_KP_4 = 0;
		static inline Key K_KP_5 = 0;
		static inline Key K_KP_6 = 0;
		static inline Key K_KP_7 = 0;
		static inline Key K_KP_8 = 0;
		static inline Key K_KP_9 = 0;
		static inline Key K_KP_DECIMAL = 0;
		static inline Key K_KP_DIVIDE = 0;
		static inline Key K_KP_MULTIPLY = 0;
		static inline Key K_KP_SUBTRACT = 0;
		static inline Key K_KP_ADD = 0;
		static inline Key K_KP_ENTER = 0;
		static inline Key K_KP_EQUAL = 0;
		static inline Key K_LEFT_SHIFT = 0;
		static inline Key K_LEFT_CONTROL = 0;
		static inline Key K_LEFT_ALT = 0;
		static inline Key K_LEFT_SUPER = 0;
		static inline Key K_LEFT_WIN = 0; // should be K_LEFT_SUPER alternative
		static inline Key K_RIGHT_SHIFT = 0;
		static inline Key K_RIGHT_CONTROL = 0;
		static inline Key K_RIGHT_ALT = 0;
		static inline Key K_RIGHT_SUPER = 0;
		static inline Key K_RIGHT_WIN = 0; // should be K_RIGHT_SUPER alternative
		static inline Key K_MENU = 0;

		// mouse
		static inline Key K_MB1 = 0;
		static inline Key K_MB2 = 0;
		static inline Key K_MB3 = 0;
		static inline Key K_MB4 = 0;
		static inline Key K_MB5 = 0;
		static inline Key K_MB6 = 0;
		static inline Key K_MB7 = 0;
		static inline Key K_MB8 = 0;
		static inline Key K_MBL = 0; // left
		static inline Key K_MBM = 0; // middle
		static inline Key K_MBR = 0; // right
	};

	enum KeyState {
		RELEASED, // normal/not pressed at all
		PRESS, // just pressed
		DOWN, // held down
		RELEASE, // just released
	};

	KeyState transitionKeyState(KeyState current, KeyState newS);
#ifdef BK_INCLUDE_GLFW
	KeyState getKeyStateFromGLFW(int32_t glfw);
#endif
}
