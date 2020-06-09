#include "CoreEngine.h"
#include "Debug.h"

namespace Abstract {

	CoreEngine::CoreEngine(Window* window, Application* scene, Delta frameTime, bool vSync)
	{
		this->window = window;
		this->scene = scene;
		this->frameTime = (Delta)(1.0) / frameTime;
		this->vSync = vSync;
		this->refreshRateTime = (Delta)(1.0) / (Delta)(window->getRefreshRate());
	}

	void CoreEngine::start()
	{
		bool isRunning = true;

		Time startTime = Clock::now();

		Delta unprocessedTime = 0.0;
		Delta renderTime = 0.0;

		int updateFrames = 0;
		int renderFrames = 0;
		int fixedFrames = 0;
		Delta frameCounter = 0.0;

		while (isRunning)
		{
			bool countFixedFrame = false;

			Time currentTime = Clock::now();
			Delta passedTime = toSeconds(currentTime - startTime);
			startTime = currentTime;

			unprocessedTime += passedTime;
			renderTime += passedTime;
			frameCounter += passedTime;

			Delta updateDelta = passedTime;
			Delta fixedDelta = frameTime;
			Delta renderDelta = vSync ? refreshRateTime : passedTime;

			//Time Independant Updating. Use "updateDelta" for delta!
			window->pollInputs();
			scene->updateScene(updateDelta);
			window->updateInput();

			while (unprocessedTime >= frameTime)
			{
				if (window->shouldClose() || scene->shouldStopEngine()) //Put in here to finish Updating last frame
				{
					isRunning = false;
				}

				//Fixed Updating. Use "fixedDelta" for delta!
				scene->fixedUpdateScene(fixedDelta);
				unprocessedTime -= frameTime;
				countFixedFrame = true;
			}

			updateFrames++;

			if (countFixedFrame)
			{
				fixedFrames++;
			}

			if (frameCounter >= 1.0)
			{
				DEBUG_LOG("Core Engine", Debug::Priority::Profile, "Update FPS: %d  Fixed FPS: %d Render FPS: %d\nTrue Delta: %.7f  Fixed Delta: %f  Render Delta: %f\n",
					updateFrames, fixedFrames, renderFrames, updateDelta, fixedDelta, renderDelta);
				updateFrames = 0;
				renderFrames = 0;
				fixedFrames = 0;
				frameCounter = 0.0;
			}

			//Render on Fixed Frames if vSync is enabled else always render. Use "renderDelta" for delta!
			if ((vSync && (renderTime >= refreshRateTime)) || !vSync)
			{
				scene->renderScene(renderDelta);
				window->present();
				renderFrames++;
				renderTime = 0.0;
			}
		}
	}

}