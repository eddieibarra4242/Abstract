#pragma once
#include "Platform.h"
#include "Time.h"
#include "../ecs/ECS.h"

namespace Abstract {

	class ABSTRACT_API Window;

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

	Window* CreateWindow();
	Application* CreateApplication();
}