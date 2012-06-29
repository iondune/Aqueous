#include "CMainState.h"

void CMainState::init()
{
	///SDL_WM_SetCaption("Underwater Volume Data Rendering... In Space!", "");

	glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void CMainState::initScene()
{
	LightPosition = SVector3f(0.2f, 0.4f, 0.2f);

	SceneManager.setActiveCamera(Camera = new CCameraControl(SVector3f(1.f, 0.3f, 1.5f)));
	Camera->setVelocity(1.9f);

	// Load dino model and apply texture
	CMesh * TyraMesh = CMeshLoader::loadObjMesh("Tyra.obj");
	TyraMesh->centerMeshByExtents(SVector3f());
	Tyra = SceneManager.addMeshSceneObject(TyraMesh, CShaderLoader::loadShader("NormalMap"), 0, CRenderable::SMaterial());
	Tyra->setCullingEnabled(false);
	Tyra->addUniform("uLightPosition", boost::shared_ptr<IUniform const>(& BindLightPosition));
	CTexture * Texture = CTextureLoader::loadTexture("TyraNormals.bmp");
	Tyra->setTexture(0, Texture);
	Tyra->setVisible(false);

	// Add space backdrop
	Cube = CMeshLoader::createCubeMesh();
	SkyBox = SceneManager.addMeshSceneObject(Cube, CShaderLoader::loadShader("DiffuseTexture"), 0);
	SkyBox->setScale(SVector3f(20.f));
	SkyBox->setTexture(0, "Space.bmp");
	SkyBox->setCullingEnabled(false);
	//SkyBox->setVisible(false);

	// Add cube to show light location
	LightObject = SceneManager.addMeshSceneObject(Cube, CShaderLoader::loadShader("Simple"), 0);
	LightObject->setScale(SVector3f(0.09f));

	Shader = CShaderLoader::loadShader("Diffuse");

	VoxelObject = new ISceneObject();
	VoxelObject->setVisible(false);
	SceneManager.addSceneObject(VoxelObject);
	VoxelObject->setCullingEnabled(false);

	SoupObject = new ISceneObject();
	SoupObject->setVisible(false);
	SceneManager.addSceneObject(SoupObject);
	SoupObject->setCullingEnabled(false);
}
