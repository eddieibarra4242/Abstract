#pragma once
#include "../core/Platform.h"
#include "../ecs/EcsSystem.h"
#include "Input.h"

namespace Abstract {

	struct ABSTRACT_API Inputs : public ECSComponent<Inputs>
	{
		IInput* input;

		Key forward;
		Key back;
		Key left;
		Key right;

		Mouse lock;
		Mouse unlock;
	};

	class ABSTRACT_API FreeMove : public BaseECSSystem
	{
	public:
		FreeMove(float speed = 1);
		virtual void onUpdate(Delta delta, BaseECSComponent** components) override;
	private:
		float speed;
	};

	class ABSTRACT_API FreeLook : public BaseECSSystem
	{
	public:
		FreeLook(float sensitivity = 1);
		virtual void onUpdate(Delta delta, BaseECSComponent** components) override;
	private:
		bool locked;
		float sensitivity;

		Vector3 up;
	};

}