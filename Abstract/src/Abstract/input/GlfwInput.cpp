#include "GlfwInput.h"

namespace Abstract {

	GlfwInput::GlfwInput(GLFWwindow* inputSource, int width, int height)
	{
		this->inputSource = inputSource;
		this->mousePosition = Vector2(0, 0);
		this->mouseDelta = Vector2(0, 0);
		this->hasBeenUpdated = false;
		this->halfWidth = (double)width / 2.0;
		this->halfHeight = (double)height / 2.0;
		this->mouseX = 0;
		this->mouseY = 0;
	}

	GlfwInput::~GlfwInput()
	{
		for (std::map<JoyStick, std::pair<const float*, size_t>*>::iterator it = joystickAxes.begin(); it != joystickAxes.end(); it++)
		{
			delete it->second;
		}

		for (std::map<JoyStick, std::pair<const uint8_t*, size_t>*>::iterator it = joystickButtons.begin(); it != joystickButtons.end(); it++)
		{
			delete it->second;
		}
	}

	void GlfwInput::initJoystick(JoyStick joystick)
	{
		if (joystickAxes.count(joystick) == 0)
		{
			joystickAxes.emplace(joystick, new std::pair<const float*, size_t>());
			joystickButtons.emplace(joystick, new std::pair<const uint8_t*, size_t>());
			updateJoystick(joystick);
		}
	}

	void GlfwInput::updateJoystick(JoyStick joystick)
	{
		size_t numAxes = 0;
		size_t numButtons = 0;

		const float* newAxesPos = glfwGetJoystickAxes((int)joystick, (int*)&numAxes);
		const uint8_t* newButtonPos = glfwGetJoystickButtons((int)joystick, (int*)&numButtons);

		joystickAxes[joystick]->first = newAxesPos;
		joystickAxes[joystick]->second = numAxes;

		joystickButtons[joystick]->first = newButtonPos;
		joystickButtons[joystick]->second = numButtons;
	}

	void GlfwInput::updateJoysticks()
	{
		for (std::map<JoyStick, std::pair<const float*, size_t>*>::iterator it = joystickAxes.begin(); it != joystickAxes.end(); ++it)
		{
			updateJoystick(it->first);
		}
	}

	void GlfwInput::updateMouse()
	{
		Vector2 mp = mousePosition;
		glfwGetCursorPos(inputSource, &mouseX, &mouseY);
		mousePosition.setX((float)(mouseX / halfWidth) - 1);
		mousePosition.setY((float)(mouseY / halfHeight) - 1);

		if (hasBeenUpdated)
		{
			mouseDelta = mousePosition - mp;
		}

		hasBeenUpdated = true;
	}

	void GlfwInput::update()
	{
		updateMouse();
		updateJoysticks();

		lastKeys.clear();
		lastMouse.clear();

		for (int i = 0; i < (int)Key::LAST; i++)
		{
			Key ind = (Key)i;
			if (getKey(ind))
			{
				lastKeys.emplace(ind);
			}
		}

		for (int i = 0; i < (int)Mouse::BUTTON_LAST; i++)
		{
			Mouse ind = (Mouse)i;
			if (getMouse(ind))
			{
				lastMouse.emplace(ind);
			}
		}
	}

	bool GlfwInput::getKey(const Key& code)
	{
		return glfwGetKey(inputSource, (int)code) == GLFW_PRESS;
	}

	bool GlfwInput::getKeyDown(const Key& code)
	{
		return getKey(code) && lastKeys.find(code) == lastKeys.end();
	}

	bool GlfwInput::getKeyUp(const Key& code)
	{
		return !getKey(code) && lastKeys.find(code) != lastKeys.end();
	}

	bool GlfwInput::getMouse(const Mouse& button)
	{
		return glfwGetMouseButton(inputSource, (int)button) == GLFW_PRESS;
	}

	bool GlfwInput::getMouseDown(const Mouse& button)
	{
		return getMouse(button) && lastMouse.find(button) == lastMouse.end();
	}

	bool GlfwInput::getMouseUp(const Mouse& button)
	{
		return !getMouse(button) && lastMouse.find(button) != lastMouse.end();
	}

	double GlfwInput::getMouseX()
	{
		return mouseX;
	}

	double GlfwInput::getMouseY()
	{
		return mouseY;
	}

	Vector2& GlfwInput::getMousePos()
	{
		return mousePosition;
	}

	double GlfwInput::getMouseDeltaX()
	{
		return mouseDelta.getX();
	}

	double GlfwInput::getMouseDeltaY()
	{
		return mouseDelta.getY();
	}

	Vector2& GlfwInput::getMouseDelta()
	{
		return mouseDelta;
	}

	std::string GlfwInput::getJoystickName(const JoyStick& joystick)
	{
		return glfwGetJoystickName((int)joystick);
	}

	size_t GlfwInput::getNumJoystickAxes(const JoyStick& joystick)
	{
		initJoystick(joystick);
		return joystickAxes[joystick]->second;
	}

	double GlfwInput::getJoystickAxis(const JoyStick& joystick, uint32_t axis)
	{
		size_t numAxis = getNumJoystickAxes(joystick);
		if (axis < 0 || axis >= numAxis)
		{
			return 0.0;
		}
		return joystickAxes[joystick]->first[axis];
	}

	size_t GlfwInput::getNumJoystickButtons(const JoyStick& joystick)
	{
		initJoystick(joystick);
		return joystickButtons[joystick]->second;
	}

	bool GlfwInput::getJoystickButton(const JoyStick& joystick, uint32_t button)
	{
		size_t numButtons = getNumJoystickButtons(joystick);
		if (button < 0 || button >= numButtons)
		{
			return 0.0;
		}
		return joystickButtons[joystick]->first[button] == 1;
	}

	void GlfwInput::setMousePos(double x, double y)
	{
		glfwSetCursorPos(inputSource, x * halfWidth + halfWidth, y * halfHeight + halfHeight);
	}

	void GlfwInput::setMouse(bool isEnabled)
	{
		glfwSetInputMode(inputSource, GLFW_CURSOR, (isEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN));
	}

}