#pragma once

#include "./Abstract/core/Platform.h"
#include "./Abstract/core/Application.h"
#include "./Abstract/core/Debug.h"
#include "./Abstract/core/Time.h"

#include "./Abstract/ecs/Ecs.h"

#include "./Abstract/input/Controls.h"
#include "./Abstract/input/Input.h"

#include "./Abstract/math/Vector.h"
#include "./Abstract/math/Quaternion.h"
#include "./Abstract/math/Matrix.h"
#include "./Abstract/math/Transform.h"

#include "./Abstract/rendering/Window.h"
#include "./Abstract/rendering/Camera.h"
#include "./Abstract/rendering/Lights.h"
#include "./Abstract/rendering/Material.h"
#include "./Abstract/rendering/Mesh.h"
#include "./Abstract/rendering/RenderEngine.h"

// only define in entry point of Game Project
// #define AB_ENTRY

#ifdef AB_ENTRY

#include "Abstract/core/CoreEngine.h"

#ifdef AB_PLATFORM_WINDOWS

extern Abstract::Window* Abstract::CreateWindow();
extern Abstract::Application* Abstract::CreateApplication();

int main(int args, char** argv)
{
	Abstract::Window* window = Abstract::CreateWindow();
	Abstract::Application* app = Abstract::CreateApplication();

	Abstract::CoreEngine engine(window, app, 60, true);
	engine.start();

	delete app;
	delete window;
}

#endif

#endif