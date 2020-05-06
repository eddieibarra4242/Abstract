#pragma once
#include "../core/Platform.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include "../math/Vector.h"
#include "Input.h"
#include <GLFW/glfw3.h>

namespace Abstract {

	class ABSTRACT_API GlfwInput : public IInput
	{
	public:
		GlfwInput(GLFWwindow* inputSource, int width, int height);

		~GlfwInput();

		/** Updates the input system */
		virtual void update() override;

		/**
		 * Gets whether or not a particular key is currently pressed.
		 *
		 * @param code
		 *            The key to test
		 * @return Whether or not key is currently pressed.
		 */
		virtual bool getKey(const Key& code) override;

		virtual bool getKeyDown(const Key& code) override;

		virtual bool getKeyUp(const Key& code) override;

		/**
		 * Gets whether or not a particular mouse button is currently pressed.
		 *
		 * @param button
		 *            The button to test
		 * @return Whether or not the button is currently pressed.
		 */
		virtual bool getMouse(const Mouse& button) override;

		virtual bool getMouseDown(const Mouse& button) override;

		virtual bool getMouseUp(const Mouse& button) override;

		/**
		 * Gets the location of the mouse cursor on x, in pixels.
		 *
		 * @return The location of the mouse cursor on x, in pixels
		 */
		virtual double getMouseX() override;

		/**
		 * Gets the location of the mouse cursor on y, in pixels.
		 *
		 * @return The location of the mouse cursor on y, in pixels
		 */
		virtual double getMouseY() override;

		virtual Vector2& getMousePos() override;

		/**
		 * Gets the amount the mouse has moved since the previous update on X.
		 *
		 * @return The amount the mouse has moved since the previous update on X.
		 */
		virtual double getMouseDeltaX() override;

		/**
		 * Gets the amount the mouse has moved since the previous update on Y.
		 *
		 * @return The amount the mouse has moved since the previous update on Y.
		 */
		virtual double getMouseDeltaY() override;

		virtual Vector2& getMouseDelta() override;

		/**
		 * Gets a string describing a particular joystick.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @return A string describing the specified joystick.
		 */
		virtual std::string getJoystickName(const JoyStick& joystick) override;

		/**
		 * Gets the number of axes a joystick offers.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @return The number of axes the joystick offers.
		 */
		virtual size_t getNumJoystickAxes(const JoyStick& joystick) override;

		/**
		 * Gets the value of a joystick's axis.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @param axis
		 *            The axis of interest.
		 * @return The value of the joystick's axis.
		 */
		virtual double getJoystickAxis(const JoyStick& joystick, uint32_t axis) override;

		/**
		 * Gets the number of buttons a joystick offers.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @return The number of buttons the joystick offers.
		 */
		virtual size_t getNumJoystickButtons(const JoyStick& joystick) override;

		/**
		 * Gets the whether a button on a joystick is pressed.
		 *
		 * @param joystick
		 *            The joystick of interest.
		 * @param button
		 *            The button of interest.
		 * @return Whether a button on a joystick is pressed.
		 */
		virtual bool getJoystickButton(const JoyStick& joystick, uint32_t button) override;

		virtual void setMousePos(double x, double y) override;

		virtual void setMouse(bool isEnabled) override;
	private:
		std::set<Key> lastKeys;
		std::set<Mouse> lastMouse;

		double mouseX;
		double mouseY;

		double halfWidth;
		double halfHeight;

		Vector2 mousePosition;
		Vector2 mouseDelta;

		bool hasBeenUpdated;

		std::map<JoyStick, std::pair<const float*, size_t>*> joystickAxes;
		std::map<JoyStick, std::pair<const uint8_t*, size_t>*> joystickButtons;

		GLFWwindow* inputSource;

		void initJoystick(JoyStick joystick);
		void updateJoystick(JoyStick joystick);
		void updateJoysticks();
		void updateMouse();
	};

}