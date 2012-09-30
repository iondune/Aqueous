#include "CLoadState.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CGwenEventForwarder.h"

#include "SciDataManager.h"
#include "CTerrainSceneObject.h"
#include "CVolumeSceneObject.h"


void CLoadStateEventHandler::OnFinish(Gwen::Controls::Base * Control)
{
	CLoadState::get().OnFinish();
}


void CLoadState::addLabel(std::wstring const & Label, Gwen::Color const & Color)
{
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Canvas);
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(Label);
	MediumLabel->SetBounds(20 + Indent, LabelHeight, 1024, 300);
	MediumLabel->SetTextColor(Color);

	GUIManager->draw(true);
	CApplication::get().swapBuffers();

	LabelHeight += 40;
}

CLoadState::CLoadState()
	: LabelHeight(0), Indent(0), GetConfirmation(false)
{
	LabelHeight = 70;
	Indent = 0;
}

void CLoadState::begin()
{
	// Load References
	CApplication & Application = CApplication::get();
	CMainState & MainState = CMainState::get();
	CMainMenuState & MenuState = CMainMenuState::get();

	GUIManager = Context->GUIContext;
	Canvas = GUIManager->getCanvas();
	
	// Init Canvas
	Canvas->SetBackgroundColor(Gwen::Color(32, 48, 48));
	Canvas->SetDrawBackground(true);

	// Top Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(Canvas);
	BigLabel->SetFont(GUIManager->getLargeFont());
	BigLabel->SetText(L"Loading...");
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(255, 255, 255, 84));

	GUIManager->draw(true);
	CApplication::get().swapBuffers();
	
	addLabel(L"Initializing System...");
	CGwenEventForwarder * Forwarder = new CGwenEventForwarder(GUIManager->getCanvas());
	
	addLabel(L"Loading Scene Shaders...");
	Application.getSceneManager().init(true, true);
	loadShaders();
	
	addLabel(L"Loading Scene Objects...");
	loadScene();
	
	Context->DataManager = new SciDataManager();
	addLabel(L"Menu is Starting...");

	if (GetConfirmation)
	{
		Gwen::Controls::Button * Button = new Gwen::Controls::Button(GUIManager->getCanvas());
		Button->SetBounds(250, 650, 250, 35);
		Button->SetText(L"Continue");
		Button->onPress.Add(& Handler, & CLoadStateEventHandler::OnFinish);
	}
	else
		OnFinish();
}

void CLoadState::OnRenderStart(float const Elapsed)
{
	Context->GUIContext->draw(Elapsed, true);
	CApplication::get().swapBuffers();
}

void CLoadState::loadShaders()
{
	Indent = 60;
	bool Failed = false;

	if (! (Context->Shaders.Glyph = CShaderLoader::loadShader("Glyph")))
		addLabel(L"Failed to load Glyph Shader - Glyphs will not draw.", Gwen::Color(255, 32, 32, 192)), Failed = true;
	if (! (Context->Shaders.DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture")))
		addLabel(L"Failed to load Diffuse/Texture Shader - Backdrop will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Volume = CShaderLoader::loadShader("Volume2")))
		addLabel(L"Failed to load Volume Shader - Volume will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Terrain = CShaderLoader::loadShader("Terrain")))
		addLabel(L"Failed to load Terrain Shader - Terrain will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;

	if (! Failed)
		addLabel(L"All shaders compiled successfully.", Gwen::Color(64, 255, 64, 192));
	else
		GetConfirmation = true;

	Indent = 0;
}

void CLoadState::loadScene()
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
	Scene.Camera = new CCameraControl(SVector3f(1.f, 0.3f, 4.5f));
	Scene.Camera->setProjection(60.f, CApplication::get().getAspectRatio(), 0.01f, 100.f);
	Scene.Camera->setVelocity(1.9f);
	SceneManager->setActiveCamera(Scene.Camera);

	Scene.OrbitCamera = new CPerspectiveCameraSceneObject();

	// Basic Shader/Mesh
	Scene.Cube = CMeshLoader::createCubeMesh();

	// Backdrop/SkyCube
	
	Scene.SkyBox = new CMeshSceneObject();
	Scene.SkyBox->setMesh(Scene.Cube);
	Scene.SkyBox->setShader(SceneManager->getDefaultColorRenderPass(), Context->Shaders.DiffuseTexture);
	Scene.SkyBox->setScale(SVector3f(28.f));
	Scene.SkyBox->setTexture(0, "Space.bmp");
	Scene.SkyBox->setCullingEnabled(false);
	Scene.SkyBox->setVisible(false);

	// Light Tracker
	//Scene.LightObject = SceneManager->addMeshSceneObject(Scene.Cube, CShaderLoader::loadShader("Simple"), 0);
	//Scene.LightObject->setScale(SVector3f(0.09f));

	// Container Objects
	Scene.PointCloudObject = new ISceneObject();
	Scene.PointCloudObject->setVisible(false);
	Scene.PointCloudObject->setCullingEnabled(false);
	SceneManager->addSceneObject(Scene.PointCloudObject);

	Scene.FloorSamplesObject = new ISceneObject();
	Scene.FloorSamplesObject->setVisible(false);
	Scene.FloorSamplesObject->setCullingEnabled(false);
	SceneManager->addSceneObject(Scene.FloorSamplesObject);

	// Terrain
	Scene.Terrain = new CTerrainSceneObject();
	SceneManager->addSceneObject(Scene.Terrain);
	Scene.Terrain->setCullingEnabled(false);
	//Scene.Terrain->setScale(SVector3f(0.0186f) * 2.f);
	//SVector3f Scale = Scene.Terrain->getScale();
	//Scale.X *= -1;
	vec3f Scale = vec3f(1.f);
	Scale /= 512.f;
	Scale *= 3.f;
	Scale.X *= -1;


	vec2f DataRangeMin(9.53894f, 63.59233f), DataRangeMax(9.54926f, 63.59595f);
	vec2f MapRangeMin(9.51013f, 63.57518f), MapRangeMax(9.56290f, 63.60297f);
	vec2f DataRangeCenter = (DataRangeMin + DataRangeMax) / 2.f;
	vec2f ScaleAdjust = (MapRangeMax - MapRangeMin) / (DataRangeMax - DataRangeMin);
	vec2f TranslationAdjust = (DataRangeCenter - MapRangeMin) / (MapRangeMax - DataRangeCenter);
	//TranslationAdjust /= 2.f;
	//TranslationAdjust -= vec2f(0.5f);
	//printf("Translation Adjust: %f %f\n", TranslationAdjust.X, TranslationAdjust.Y);
	vec2f RelativeTranslate;
	for (int i = 0; i < 2; ++ i)
	{
		if (TranslationAdjust[i] > 1.f)
		{
			TranslationAdjust[i] = 1.f / TranslationAdjust[i];

			RelativeTranslate[i] = -(1.f - TranslationAdjust[i]);
		}
		else
		{
			RelativeTranslate[i] = (1.f - TranslationAdjust[i]);
		}
	}

	f32 MaxAdjust;
	vec3f Adjuster(1.f);
	if (ScaleAdjust.X > ScaleAdjust.Y)
	{
		Adjuster.Z = ScaleAdjust.X / ScaleAdjust.Y;
		MaxAdjust = ScaleAdjust.X;
	}
	else
	{
		Adjuster.X = ScaleAdjust.Y / ScaleAdjust.X;
		MaxAdjust = ScaleAdjust.Y;
	}

	RelativeTranslate *= MaxAdjust;//ScaleAdjust;
	
	Scale.X *= MaxAdjust;//ScaleAdjust.X;
	Scale.Z *= MaxAdjust;//ScaleAdjust.Y;
	Scale.Y *= (ScaleAdjust.X + ScaleAdjust.Y) / 2.f;

	Scale.Y *= 0.5f;
	Scene.Terrain->setScale(Scale);
	Scene.Terrain->setTranslation(vec3f(-RelativeTranslate.X, 0.f, RelativeTranslate.Y));
	//printf("Relative Translate: %f %f\n", RelativeTranslate.X, RelativeTranslate.Y);
	//Scene.Terrain->setTranslation(SVector3f(2.026057f, -0.5f, -4.140311f));
	//Scene.Terrain->setRotation(SVector3f(0.f, 10.f, 0.f));
	Scene.Terrain->setVisible(false);

	// Volume
	Scene.VolumeSceneObject = new CVolumeSceneObject();
	//SceneManager->addSceneObject(Scene.VolumeSceneObject);
	Scene.VolumeSceneObject->ShowVolume = 2;
	//Scene.VolumeSceneObject->setScale(Adjuster * 3.f * vec3f(1.f, 0.8f / 3.f, 1.f));

	//Scene.PointCloudObject->setScale(Adjuster * vec3f(3.f, 0.8f, 3.f));
	Scene.PointCloudObject->setScale(vec3f(3.f, 1.5f, 3.f));
	Scene.VolumeSceneObject->setScale(vec3f(3.f, 1.5f, 3.f));
}

void CLoadState::OnFinish()
{
	// Cleanup GUI
	Canvas->RemoveAllChildren();

	Application->getStateManager().setState(& CMainMenuState::get());
}
