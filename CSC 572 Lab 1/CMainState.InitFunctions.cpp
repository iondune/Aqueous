#include "CMainState.h"

void CMainState::init()
{
	SDL_WM_SetCaption("Dinosaurs in Space!", "");

    glClearColor(0.2f, 0.2f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void CMainState::initScene()
{
	LightPosition = SVector3(0.2f, 0.4f, 0.2f);

	SceneManager.setActiveCamera(Camera = new CCameraControl(SVector3(1.f, 0.3f, 1.5f), -1.9f));

	// Load dino model and apply texture
	CMesh * TyraMesh = CMeshLoader::loadObjMesh("Tyra.obj");
	TyraMesh->centerMeshByExtents(SVector3());
	Tyra = SceneManager.addMeshSceneObject(TyraMesh, CShaderLoader::loadShader("NormalMap"), CMaterial());
	Tyra->setCullingEnabled(false);
	Tyra->addUniform("uLightPosition", & BindLightPosition);
	CTexture * Texture = CTextureLoader::loadTexture("TyraNormals.bmp");
	Tyra->setTexture(Texture);
	Tyra->setVisible(false);

	// Add space backdrop
	Cube = CMeshLoader::createCubeMesh();
	CMeshSceneObject * SkyBox = SceneManager.addMeshSceneObject(Cube, CShaderLoader::loadShader("DiffuseTexture"));
	SkyBox->setScale(SVector3(20.f));
	SkyBox->setTexture("Space.bmp");
	SkyBox->setCullingEnabled(false);
	SkyBox->setVisible(false);

	// Add cube to show light location
	LightObject = SceneManager.addMeshSceneObject(Cube, CShaderLoader::loadShader("Simple"));
	LightObject->setScale(SVector3(0.09f));

	Shader = CShaderLoader::loadShader("Diffuse");

	VoxelObject = new ISceneObject();
	SceneManager.addSceneObject(VoxelObject);
	VoxelObject->setCullingEnabled(false);

	SoupObject = new ISceneObject();
	SoupObject->setVisible(false);
	SceneManager.addSceneObject(SoupObject);
	SoupObject->setCullingEnabled(false);
}