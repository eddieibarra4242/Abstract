#include "Controls.h"
#include "../math/Transform.h"
#include "../rendering/Camera.h"

namespace Abstract {

	FreeMove::FreeMove(float speed)
	{
		addComponentType(TransformComp::ID);
		addComponentType(Inputs::ID);

		this->speed = speed;
	}

	void FreeMove::onUpdate(Delta delta, BaseECSComponent** components)
	{
		Transform& transform = ((TransformComp*)components[0])->transform;
		Inputs* inputs = (Inputs*)components[1];

		if (inputs->input->getKey(inputs->forward))
		{
			transform.move(transform.getRotation().getForward() * speed * delta);
		}

		if (inputs->input->getKey(inputs->back))
		{
			transform.move(transform.getRotation().getBack() * speed * delta);
		}

		if (inputs->input->getKey(inputs->left))
		{
			transform.move(transform.getRotation().getLeft() * speed * delta);
		}

		if (inputs->input->getKey(inputs->right))
		{
			transform.move(transform.getRotation().getRight() * speed * delta);
		}
	}

	FreeLook::FreeLook(float sensitivity)
	{
		addComponentType(TransformComp::ID);
		addComponentType(Inputs::ID);

		this->locked = false;
		this->sensitivity = sensitivity;
		this->up = Vector3(0, 1, 0);
	}

	void FreeLook::onUpdate(Delta delta, BaseECSComponent** components)
	{
		Transform& transform = ((TransformComp*)components[0])->transform;
		Inputs* inputs = (Inputs*)components[1];

		if (!locked && inputs->input->getMouseDown(inputs->lock))
		{
			locked = true;
			inputs->input->setMouse(false);
			inputs->input->setMousePos(0, 0);
		}

		if (locked && inputs->input->getMouseDown(inputs->unlock))
		{
			locked = false;
			inputs->input->setMouse(true);
		}

		if (locked)
		{
			Vector2 deltaMouse = inputs->input->getMousePos() * 500;

			bool rotY = deltaMouse.getX() != 0;
			bool rotX = deltaMouse.getY() != 0;

			if (rotY)
			{
				transform.rotate(up, deltaMouse.getX() * sensitivity * delta);
			}

			if (rotX)
			{
				transform.rotate(transform.getRotation().getRight(), -deltaMouse.getY() * sensitivity * delta);
			}

			if (rotY || rotX)
			{
				inputs->input->setMousePos(0, 0);
			}
		}
	}

}