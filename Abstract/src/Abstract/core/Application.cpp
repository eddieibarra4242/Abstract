#include "Application.h"

namespace Abstract {

	Application::Application()
	{
		m_stopEngine = false;
		ecs = new ECS();
	}

	Application::~Application()
	{
		delete ecs;
	}

	void Application::addSystem(BaseECSSystem& system, SystemType type)
	{
		switch (type)
		{
		case SystemType::UPDATE: updateSystems.addSystem(system); return;
		case SystemType::FIXED: fixedUpdateSystems.addSystem(system); return;
		case SystemType::RENDER: renderSystems.addSystem(system); return;
		}
	}

	void Application::stopEngine()
	{
		m_stopEngine = true;
	}

	void Application::updateScene(Delta delta)
	{
		update(delta);
		ecs->updateSystems(updateSystems, delta, ECS::UpdateType::WHENEVER);
	}

	void Application::fixedUpdateScene(Delta delta)
	{
		fixedUpdate(delta);
		ecs->updateSystems(fixedUpdateSystems, delta, ECS::UpdateType::FIXED);
	}

	void Application::renderScene(Delta delta)
	{
		render(delta);
		ecs->updateSystems(renderSystems, delta, ECS::UpdateType::RENDER);
	}

}