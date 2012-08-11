#include "CLoadContext.h"

#include "CMainState.h"
#include "CGwenEventForwarder.h"


void CLoadContext::addLabel(std::wstring const & Label)
{
	//std::cout << "1" << std::flush;
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Canvas);
	//std::cout << "2" << std::flush;
	MediumLabel->SetFont(GUIManager->getMediumFont());
	//std::cout << "3" << std::flush;
	MediumLabel->SetText(Label);
	//std::cout << "4" << std::flush;
	MediumLabel->SetBounds(20 + Indent, LabelHeight, 600, 300);
	//std::cout << "5" << std::flush;
	MediumLabel->SetTextColor(Gwen::Color(255, 255, 255, 84));
	//std::cout << "6" << std::endl;

	GUIManager->draw(true);
	CApplication::get().swapBuffers();

	LabelHeight += 40;
}

void CLoadContext::setProgress(f32 const progress)
{
	Progress->SetValue(progress);
}

CLoadContext::CLoadContext()
	: LabelHeight(0), Indent(0)
{}

void CLoadContext::init()
{
	loadContext();

	LabelHeight = 70;
	Indent = 0;
}

void CLoadContext::run()
{
	// Load References
	CApplication & Application = CApplication::get();
	CMainState & MainState = CMainState::get();

	GUIManager = Context->GUIContext;
	Canvas = GUIManager->getCanvas();
	
	// Init Canvas
	Canvas->SetBackgroundColor(Gwen::Color(32, 48, 48));
	Canvas->SetDrawBackground(true);

	// Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(Canvas);
	BigLabel->SetFont(GUIManager->getLargeFont());
	BigLabel->SetText(Gwen::UnicodeString(L"Loading..."));
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(255, 255, 255, 84));
	
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Canvas);
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(Gwen::UnicodeString(L"Progress:"));
	MediumLabel->SetBounds(500, 100, 500, 300);
	MediumLabel->SetTextColor(Gwen::Color(255, 255, 255, 84));

	Progress = new Gwen::Controls::ProgressBar(Canvas);
	Progress->SetBounds(500, 150, 500, 50);
	setProgress(0.1f);

	GUIManager->draw(true);
	CApplication::get().swapBuffers();
	
	setProgress(0.2f);
	addLabel(L"Initializing System...");
	Application.loadEngines();
	MainState.load();
	CGwenEventForwarder * Forwarder = new CGwenEventForwarder(GUIManager->getCanvas());
	
	setProgress(0.4f);
	addLabel(L"Loading Scene Shaders...");
	Application.getSceneManager().init();
	
	setProgress(0.6f);
	addLabel(L"Loading Scene Objects...");
	loadScene();
	
	setProgress(0.8f);
	addLabel(L"Loading Science Data...");
	loadData();
	
	setProgress(1.f);
	addLabel(L"Application is Starting...");
}

void CLoadContext::loadScene()
{
	// References
	CProgramContext::SScene & Scene = Context->Scene;
	CSceneManager * SceneManager = & CApplication::get().getSceneManager();


	// OpenGL Parameters
	glClearColor(0.3f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Global Light Position
	Scene.LightPosition = SVector3f(0.2f, 0.4f, 0.2f);

	// Cameras
	Scene.Camera = new CCameraControl(SVector3f(1.f, 0.3f, 1.5f));
	Scene.Camera->setProjection(60.f, 16.f/9.f, 0.1f, 1000.f);
	Scene.Camera->recalculateViewMatrix();
	Scene.Camera->setVelocity(1.9f);
	SceneManager->setActiveCamera(Scene.Camera);

	Scene.OrbitCamera = new CPerspectiveCameraSceneObject();

	// Basic Shader/Mesh
	Scene.Cube = CMeshLoader::createCubeMesh();
	Scene.Shader = CShaderLoader::loadShader("Diffuse");

	// Backdrop/SkyCube
	Scene.SkyBox = SceneManager->addMeshSceneObject(Scene.Cube, CShaderLoader::loadShader("DiffuseTexture"), 0);
	Scene.SkyBox->setScale(SVector3f(28.f));
	Scene.SkyBox->setTexture(0, "Space.bmp");
	Scene.SkyBox->setCullingEnabled(false);

	// Light Tracker
	Scene.LightObject = SceneManager->addMeshSceneObject(Scene.Cube, CShaderLoader::loadShader("Simple"), 0);
	Scene.LightObject->setScale(SVector3f(0.09f));

	// Container Objects
	Scene.PointCloudObject = new ISceneObject();
	Scene.PointCloudObject->setVisible(false);
	SceneManager->addSceneObject(Scene.PointCloudObject);
	Scene.PointCloudObject->setCullingEnabled(false);

	Scene.GridObject = new ISceneObject();
	Scene.GridObject->setVisible(false);
	SceneManager->addSceneObject(Scene.GridObject);
	Scene.GridObject->setCullingEnabled(false);

	// Terrain
	Scene.Terrain = new CTerrainSceneObject();
	SceneManager->addSceneObject(Scene.Terrain);
	Scene.Terrain->setCullingEnabled(false);
	Scene.Terrain->setScale(SVector3f(0.0186f) * 2.f);
	SVector3f Scale = Scene.Terrain->getScale();
	Scale.X *= -1;
	Scene.Terrain->setScale(Scale);
	Scene.Terrain->setTranslation(SVector3f(2.026057f, -0.5f, -4.140311f));
	Scene.Terrain->setRotation(SVector3f(0.f, 10.f, 0.f));

	// Volume
	Scene.VolumeSceneObject = new CVolumeSceneObject();
	SceneManager->addSceneObject(Scene.VolumeSceneObject);
}
