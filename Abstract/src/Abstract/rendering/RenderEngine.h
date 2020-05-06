#pragma once
#include "../ecs/EcsSystem.h"
#include "Shader.h"
#include "../core/Time.h"
#include "../core/Platform.h"

namespace Abstract {

	class ABSTRACT_API RenderingEngine
	{
	public:
		RenderingEngine(const RenderingEngine&) = delete;
		RenderingEngine operator=(const RenderingEngine&) = delete;

		inline static RenderingEngine& get()
		{
			static RenderingEngine instance;
			return instance;
		}

		//methods ...

	private:
		RenderingEngine();

		//Values ...
	};

	class ABSTRACT_API CameraInfoGetter : public BaseECSSystem
	{
	public:
		CameraInfoGetter();
		virtual void onRender(Delta delta, BaseECSComponent** components) override;
		static Matrix4 viewProj;
	private:
	};

	class ABSTRACT_API MeshRenderer : public BaseECSSystem
	{
	public:
		MeshRenderer();
		virtual void preFrameRender(Delta delta) override;
		virtual void onRender(Delta delta, BaseECSComponent** components) override;
		virtual void postFrameRender(Delta delta) override;
	private:
		Shader basic;

		Delta outputTime;
		Delta accum;
		uint32_t count;

		Time start;
		Time end;
	};

}