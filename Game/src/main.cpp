#define AB_ENTRY
#include <Abstract.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using namespace Abstract;

Window* context = nullptr;

class TestGame : public Application
{
public:
	TestGame();
	~TestGame() { }
protected:
	virtual void update(Delta delta) override;
	virtual void fixedUpdate(Delta delta) override;
	virtual void render(Delta delta) override;

private:
	IInput* input;

	//camera controller crap
	FreeMove mover;
	FreeLook looker;

	//eventually move these into scene since these define the rendering engine
	CameraInfoGetter getter;
	MeshRenderer meshRenderer;

	EntityHandle tri;
	EntityHandle tri2;
};

TestGame::TestGame() : Application(), getter(), meshRenderer(), mover(4), looker(6)
{
	if (context == nullptr)
	{
		CreateWindow();
	}

	input = context->getInput();

	IndexedModel floor = RenderTools::loadModelFromObj("./res/models/uvcube.obj");
	IndexedModel cube = RenderTools::loadModelFromObj("./res/models/monkey3.obj");

	Mesh mesh1 = cube.finalizeToMesh();
	Mesh mesh2 = floor.finalizeToMesh();
	Camera camera;
	camera.projection = Matrix4().setPerspective(90 * 3.14159265358979f / 180, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);

	TransformComp tri2Transform;
	tri2Transform.transform = Transform(Vector3(-2, 0, 3), Quaternion());

	TransformComp playerTransform;
	playerTransform.transform = Transform();

	Material material = Material();
	material.texture = Texture("./res/textures/bricks.jpg");
	material.specIntensity = 1;
	material.specPower = 8;

	Material material2 = Material();
	material2.texture = Texture("./res/textures/wood.jpg");
	material2.specIntensity = 1;
	material2.specPower = 8;

	Inputs inputs;
	inputs.input = context->getInput();
	inputs.forward = Key::W;
	inputs.back = Key::S;
	inputs.left = Key::A;
	inputs.right = Key::D;
	inputs.lock = Mouse::BUTTON_LEFT;
	inputs.unlock = Mouse::BUTTON_RIGHT;

	EntityHandle player = getECS()->makeEntity(camera, playerTransform, inputs);

	//for (uint32_t i = 0; i < 1000; i++)
	//{
	//	int x = (i % 100) % 10 - 5;
	//	int y = i / 100;
	//	int z = (i % 100) / 10 - 5;

	//	TransformComp triTransform;
	//	triTransform.transform = Transform(Vector3(x, y, z) * 3, Quaternion());
	//	tri = getECS()->makeEntity(mesh1, triTransform, material);
	//}

	TransformComp triTransform;
	triTransform.transform = Transform(Vector3(2, 0, 3), Quaternion());
	tri = getECS()->makeEntity(mesh1, triTransform, material);

	tri2 = getECS()->makeEntity(mesh2, tri2Transform, material2);

	addSystem(mover, SystemType::UPDATE);
	addSystem(looker, SystemType::UPDATE);
	addSystem(getter, SystemType::RENDER);
	addSystem(meshRenderer, SystemType::RENDER);
}

void TestGame::update(Delta delta)
{
	getECS()->getComponent<TransformComp>(tri)->transform.rotate(Vector3(0, 1, 0), (float)delta);
	getECS()->getComponent<TransformComp>(tri2)->transform.rotate(Vector3(0, 1, 0), (float)-delta);

	if (input->getKey(Key::ESCAPE))
	{
		this->stopEngine();
	}
}

void TestGame::fixedUpdate(Delta delta)
{

}

void TestGame::render(Delta delta)
{

}

Window* Abstract::CreateWindow()
{
	if (context == nullptr)
	{
		context = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Game");
	}

	return context;
}

Application* Abstract::CreateApplication()
{
	Debug::setPriority(Debug::Priority::PROFILE);

	return new TestGame();
}