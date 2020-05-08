#pragma once
#include "Platform.h"
#include "Time.h"
#include "../ecs/ECS.h"

namespace Abstract {

	class ABSTRACT_API Window;

	struct ABSTRACT_API Configuration
	{
		int winWidth = 320;
		int winHeight = 240;
		std::string title = "Abstract Engine";

		bool verboseLogging = false;
		Debug::Priority engineLogPriority = Debug::Priority::ERROR;

		Delta fixedUpdateFPS = 60.0;
		bool vSync = true;
	};

	class ABSTRACT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void updateScene(Delta delta);
		void fixedUpdateScene(Delta delta);
		void renderScene(Delta delta);

		inline bool shouldStopEngine() { return m_stopEngine; }
	protected:
		enum class SystemType
		{
			UPDATE,
			FIXED,
			RENDER
		};

		virtual void update(Delta delta) { }
		virtual void fixedUpdate(Delta delta) { }
		virtual void render(Delta delta) { }

		void addSystem(BaseECSSystem& system, SystemType type);

		void stopEngine();

		inline ECS* getECS()
		{
			return ecs;
		}
	private:
		bool m_stopEngine;

		ECS* ecs;

		ECSSystemList updateSystems;
		ECSSystemList fixedUpdateSystems;
		ECSSystemList renderSystems;
	};

	Configuration* ConfigureEngine();
	Application* CreateApplication(const Window* context);
}