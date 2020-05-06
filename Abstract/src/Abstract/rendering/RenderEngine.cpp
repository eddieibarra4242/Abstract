#include "RenderEngine.h"
#include "Camera.h"
#include "../math/Transform.h"
#include "Material.h"
#include "Mesh.h"
#include "Window.h"

#include <GL/glew.h>

namespace Abstract {

	Matrix4 CameraInfoGetter::viewProj;

	RenderingEngine::RenderingEngine()
	{

	}

	CameraInfoGetter::CameraInfoGetter()
	{
		addComponentType(TransformComp::ID);
		addComponentType(Camera::ID);
	}

	void CameraInfoGetter::onRender(Delta delta, BaseECSComponent** components)
	{
		Transform& transform = ((TransformComp*)components[0])->transform;
		Camera* camera = (Camera*)components[1];

		Matrix4 posMat = Matrix4().setTranslation(-transform.getPosition());
		Matrix4 rotMat = Matrix4().setRotation(transform.getRotation().conjugate());

		viewProj = camera->projection * rotMat * posMat;
	}

	MeshRenderer::MeshRenderer()
	{
		addComponentType(Mesh::ID);
		addComponentType(Material::ID);
		addComponentType(TransformComp::ID);

		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glClearColor(0, 0.15f, 0.35f, 0);

		basic = Shader("./res/shaders/basic");
	}

	void MeshRenderer::preFrameRender(Delta delta)
	{
		start = Clock::now();

		Window::bindAsRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void MeshRenderer::onRender(Delta delta, BaseECSComponent** components)
	{
		Mesh* mesh = (Mesh*)components[0];
		Material* material = (Material*)components[1];
		Transform& transform = ((TransformComp*)components[2])->transform;

		basic.bind();
		basic.sendMatrix4("MVP", CameraInfoGetter::viewProj * transform.getModelMat());
		basic.sendMatrix4("model", transform.getModelMat());
		material->texture.bind(0);
		basic.sendTextureUnit("diffuse", 0);

		glBindVertexArray(mesh->vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexArrayBuffer);
		glEnableVertexArrayAttrib(mesh->vertexArrayObject, 0);
		glEnableVertexArrayAttrib(mesh->vertexArrayObject, 1);
		glEnableVertexArrayAttrib(mesh->vertexArrayObject, 2);
		glEnableVertexArrayAttrib(mesh->vertexArrayObject, 3);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mesh->vertexSize, (GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (GLvoid*)20);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (GLvoid*)32);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexElementBuffer);
		glDrawElements(GL_TRIANGLES, mesh->drawCount, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexArrayBuffer);
		glDisableVertexArrayAttrib(mesh->vertexArrayObject, 3);
		glDisableVertexArrayAttrib(mesh->vertexArrayObject, 2);
		glDisableVertexArrayAttrib(mesh->vertexArrayObject, 1);
		glDisableVertexArrayAttrib(mesh->vertexArrayObject, 0);

		glBindVertexArray(0);
	}

	void MeshRenderer::postFrameRender(Delta delta)
	{
		end = Clock::now();

		outputTime += delta;
		accum += toSeconds(end - start);
		count++;

		if (outputTime >= 1.0)
		{
			DEBUG_LOG("Mesh Renderer", Debug::Priority::PROFILE, "Mesh Render Time Avg: %.10f\n", accum / (Delta)count);
			outputTime = 0.0;
			accum = 0.0;
			count = 0;
		}
	}

}