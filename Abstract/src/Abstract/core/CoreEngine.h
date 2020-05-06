#pragma once

#include "Platform.h"
#include "../rendering/Window.h"
#include "Application.h"
#include "Time.h"

namespace Abstract {

	class ABSTRACT_API CoreEngine
	{
	public:
		CoreEngine(Window* window, Application* scene, Delta frameTime, bool vSync);

		void start();

		~CoreEngine() { }
	private:
		Window* window;
		Application* scene;

		Delta frameTime;

		bool vSync;
		Delta refreshRateTime;
	};

}