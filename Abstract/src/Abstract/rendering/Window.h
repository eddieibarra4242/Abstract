#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../input/GlfwInput.h"

#include "../core/Platform.h"

namespace Abstract {

	class ABSTRACT_API Window
	{
	public:
		Window(int width, int height, const std::string& title);

		void pollInputs();
		void updateInput();
		void present();
		bool shouldClose();

		inline int getRefreshRate() { return refreshRate; }
		inline IInput* getInput() { return input; }

		inline static int getWidth() { return width; }
		inline static int getHeight() { return height; }

		inline static void bindAsRenderTarget()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, width, height);
		}

		~Window();
	private:
		GLFWwindow* window;
		int refreshRate;

		static int width;
		static int height;

		IInput* input;
	};

}