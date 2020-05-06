#include "Window.h"
#include "../core/Debug.h"
#include <iostream>

namespace Abstract {

	int Window::width;
	int Window::height;

	Window::Window(int width, int height, const std::string& title)
	{
		if (glfwInit() == GLFW_FALSE)
		{
			DEBUG_LOG("Window", Debug::Priority::ERROR, "GLFW Could not initialize");
		}

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		const GLFWvidmode* vm = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(window, (vm->width - width) / 2, (vm->height - height) / 2);

		refreshRate = vm->refreshRate;

		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);
		glfwShowWindow(window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			DEBUG_LOG("Window", Debug::Priority::ERROR, "Glew Could not initialize!");
		}

		Window::width = width;
		Window::height = height;
		input = new GlfwInput(window, width, height);
	}

	void Window::pollInputs()
	{
		glfwPollEvents();
	}

	void Window::updateInput()
	{
		input->update();
	}

	void Window::present()
	{
		glfwSwapBuffers(window);
	}

	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(window) == GLFW_TRUE;
	}

	Window::~Window()
	{
		delete input;

		glfwDestroyWindow(window);
		glfwTerminate();
	}

}