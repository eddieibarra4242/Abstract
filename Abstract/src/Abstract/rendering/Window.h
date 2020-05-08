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

		constexpr int getRefreshRate() const { return refreshRate; }
		constexpr IInput* getInput() const { return input; }

		constexpr static int getWidth() { return width; }
		constexpr static int getHeight() { return height; }

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