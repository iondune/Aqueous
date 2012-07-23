#include "CMainState.h"

void CMainState::init()
{	glClearColor(0.3f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void CMainState::initScene()
{
	LightPosition = SVector3f(0.2f, 0.4f, 0.2f);

	Camera = new CCameraControl(SVector3f(1.f, 0.3f, 1.5f));
	Camera->setProjection(60.f, 16.f/9.f, 0.1f, 1000.f);
	Camera->recalculateViewMatrix();
	Camera->setVelocity(1.9f);
	SceneManager.setActiveCamera(Camera);

	OrbitCamera = new CPerspectiveCameraSceneObject();

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

	PointCloudObject = new ISceneObject();
	PointCloudObject->setVisible(false);
	SceneManager.addSceneObject(PointCloudObject);
	PointCloudObject->setCullingEnabled(false);

	GridObject = new ISceneObject();
	GridObject->setVisible(false);
	SceneManager.addSceneObject(GridObject);
	GridObject->setCullingEnabled(false);



	Terrain = new CTerrainSceneObject();
	SceneManager.addSceneObject(Terrain);
	Terrain->setCullingEnabled(false);
	Terrain->setScale(SVector3f(0.05f));
	SVector3f Scale = Terrain->getScale();
	Scale.X *= -1;
	Terrain->setScale(Scale);
	Terrain->setTranslation(SVector3f(-0.3f, -0.5f, -2.1f));
	Terrain->setRotation(SVector3f(0.f, 10.f, 0.f));

	SceneManager.addSceneObject(& VolumeSceneObject);
}
