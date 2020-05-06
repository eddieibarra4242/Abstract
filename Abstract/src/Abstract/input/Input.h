#pragma once

#include "../math/Vector.h"

namespace Abstract {

	ABSTRACT_API enum class Mouse
	{
		BUTTON_1 = 0x0, BUTTON_2 = 0x1,
		BUTTON_3 = 0x2, BUTTON_4 = 0x3, BUTTON_5 = 0x4,
		BUTTON_6 = 0x5, BUTTON_7 = 0x6, BUTTON_8 = 0x7,
		BUTTON_LAST = BUTTON_8,
		BUTTON_LEFT = BUTTON_1,
		BUTTON_RIGHT = BUTTON_2,
		BUTTON_MIDDLE = BUTTON_3,

		NUM_MOUSEBUTTONS = 16
	};

	ABSTRACT_API enum class Key
	{
		SPACE = 0x20, APOSTROPHE = 0x27,
		COMMA = 0x2C, MINUS = 0x2D, PERIOD = 0x2E,
		SLASH = 0x2F, NUM_0 = 0x30, NUM_1 = 0x31, NUM_2 = 0x32,
		NUM_3 = 0x33, NUM_4 = 0x34, NUM_5 = 0x35, NUM_6 = 0x36,
		NUM_7 = 0x37, NUM_8 = 0x38, NUM_9 = 0x39, SEMICOLON = 0x3B,
		EQUAL = 0x3D, A = 0x41, B = 0x42, C = 0x43,
		D = 0x44, E = 0x45, F = 0x46, G = 0x47,
		H = 0x48, I = 0x49, J = 0x4A, K = 0x4B,
		L = 0x4C, M = 0x4D, N = 0x4E, O = 0x4F,
		P = 0x50, Q = 0x51, R = 0x52, S = 0x53,
		T = 0x54, U = 0x55, V = 0x56, W = 0x57,
		X = 0x58, Y = 0x59, Z = 0x5A, LEFT_BRACKET = 0x5B,
		BACKSLASH = 0x5C, RIGHT_BRACKET = 0x5D,
		GRAVE_ACCENT = 0x60, WORLD_1 = 0xA1, WORLD_2 = 0xA2,

		ESCAPE = 0x100, ENTER = 0x101,
		TAB = 0x102, BACKSPACE = 0x103, INSERT = 0x104,
		DELETE = 0x105, RIGHT = 0x106, LEFT = 0x107,
		DOWN = 0x108, UP = 0x109, PAGE_UP = 0x10A,
		PAGE_DOWN = 0x10B, HOME = 0x10C, END = 0x10D,
		CAPS_LOCK = 0x118, SCROLL_LOCK = 0x119,
		NUM_LOCK = 0x11A, PRINT_SCREEN = 0x11B, PAUSE = 0x11C,
		F1 = 0x122, F2 = 0x123, F3 = 0x124, F4 = 0x125,
		F5 = 0x126, F6 = 0x127, F7 = 0x128, F8 = 0x129,
		F9 = 0x12A, F10 = 0x12B, F11 = 0x12C, F12 = 0x12D,
		F13 = 0x12E, F14 = 0x12F, F15 = 0x130, F16 = 0x131,
		F17 = 0x132, F18 = 0x133, F19 = 0x134, F20 = 0x135,
		F21 = 0x136, F22 = 0x137, F23 = 0x138, F24 = 0x139,
		F25 = 0x13A, KP_0 = 0x140, KP_1 = 0x141,
		KP_2 = 0x142, KP_3 = 0x143, KP_4 = 0x144,
		KP_5 = 0x145, KP_6 = 0x146, KP_7 = 0x147,
		KP_8 = 0x148, KP_9 = 0x149, KP_DECIMAL = 0x14A,
		KP_DIVIDE = 0x14B, KP_MULTIPLY = 0x14C,
		KP_SUBTRACT = 0x14D, KP_ADD = 0x14E, KP_ENTER = 0x14F,
		KP_EQUAL = 0x150, LEFT_SHIFT = 0x154,
		LEFT_CONTROL = 0x155, LEFT_ALT = 0x156,
		LEFT_SUPER = 0x157, RIGHT_SHIFT = 0x158,
		RIGHT_CONTROL = 0x159, RIGHT_ALT = 0x15A,
		RIGHT_SUPER = 0x15B, MENU = 0x15C, LAST = MENU,

		NUM_KEYS = 256
	};

	ABSTRACT_API enum class JoyStick
	{
		STICK_0 = 0,
		STICK_1 = 1,
		STICK_2 = 2,
		STICK_3 = 3,
		STICK_4 = 4,
		STICK_5 = 5,
		STICK_6 = 6,
		STICK_7 = 7,
		STICK_8 = 8,
		STICK_9 = 9,
		STICK_10 = 10,
		STICK_11 = 11,
		STICK_12 = 12,
		STICK_13 = 13,
		STICK_14 = 14,
		STICK_15 = 15,

		NUM_STICKS = 16
	};

	class ABSTRACT_API IInput
	{
	public:
		/** Updates the input system */
		virtual void update() = 0;

		/**
		 * Gets whether or not a particular key is currently pressed.
		 *
		 * @param code
		 *            The key to test
		 * @return Whether or not key is currently pressed.
		 */
		virtual bool getKey(const Key& code) = 0;

		virtual bool getKeyDown(const Key& code) = 0;

		virtual bool getKeyUp(const Key& code) = 0;

		/**
		 * Gets whether or not a particular mouse button is currently pressed.
		 *
		 * @param button
		 *            The button to test
		 * @return Whether or not the button is currently pressed.
		 */
		virtual bool getMouse(const Mouse& button) = 0;

		virtual bool getMouseDown(const Mouse& button) = 0;

		virtual bool getMouseUp(const Mouse& button) = 0;

		/**
		 * Gets the location of the mouse cursor on x, in pixels.
		 *
		 * @return The location of the mouse cursor on x, in pixels
		 */
		virtual double getMouseX() = 0;

		/**
		 * Gets the location of the mouse cursor on y, in pixels.
		 *
		 * @return The location of the mouse cursor on y, in pixels
		 */
		virtual double getMouseY() = 0;

		virtual Vector2& getMousePos() = 0;

		/**
		 * Gets the amount the mouse has moved since the previous update on X.
		 *
		 * @return The amount the mouse has moved since the previous update on X.
		 */
		virtual double getMouseDeltaX() = 0;

		/**
		 * Gets the amount the mouse has moved since the previous update on Y.
		 *
		 * @return The amount the mouse has moved since the previous update on Y.
		 */
		virtual double getMouseDeltaY() = 0;

		virtual Vector2& getMouseDelta() = 0;

		/**
		 * Gets a string describing a particular joystick.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @return A string describing the specified joystick.
		 */
		virtual std::string getJoystickName(const JoyStick& joystick) = 0;

		/**
		 * Gets the number of axes a joystick offers.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @return The number of axes the joystick offers.
		 */
		virtual size_t getNumJoystickAxes(const JoyStick& joystick) = 0;

		/**
		 * Gets the value of a joystick's axis.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @param axis
		 *            The axis of interest.
		 * @return The value of the joystick's axis.
		 */
		virtual double getJoystickAxis(const JoyStick& joystick, uint32_t axis) = 0;

		/**
		 * Gets the number of buttons a joystick offers.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @return The number of buttons the joystick offers.
		 */
		virtual size_t getNumJoystickButtons(const JoyStick& joystick) = 0;

		/**
		 * Gets the whether a button on a joystick is pressed.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @param button
		 *            The button of interest.
		 * @return Whether a button on a joystick is pressed.
		 */
		virtual bool getJoystickButton(const JoyStick& joystick, uint32_t button) = 0;

		virtual void setMousePos(double x, double y) = 0;

		virtual void setMouse(bool isEnabled) = 0;
	};

}