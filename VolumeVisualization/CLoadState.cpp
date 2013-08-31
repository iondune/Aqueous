#include "CLoadState.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CGUIEventManager.h"

#include "SciDataManager.h"
#include "CTerrainSceneObject.h"
#include "CVolumeSceneObject.h"
#include "CGlyphSceneObject.h"
#include "CPlaneGridSceneObject.h"


void CLoadStateEventHandler::OnFinish(Gwen::Controls::Base * Control)
{
	CLoadState::Get().OnFinish();
}


void CLoadState::addLabel(std::wstring const & Label, Gwen::Color const & Color)
{
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Canvas);
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(Label);
	MediumLabel->SetBounds(20 + Indent, LabelHeight, 1024, 300);
	MediumLabel->SetTextColor(Color);

	GUIManager->draw(true);
	CApplication::Get().GetWindow().SwapBuffers();

	LabelHeight += 40;
}

CLoadState::CLoadState()
	: LabelHeight(0), Indent(0), GetConfirmation(false)
{
	LabelHeight = 70;
	Indent = 0;
}

void CLoadState::Begin()
{
	// Load References
	CApplication & Application = CApplication::Get();
	CMainState & MainState = CMainState::Get();
	CMainMenuState & MenuState = CMainMenuState::Get();

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
	CApplication::Get().GetWindow().SwapBuffers();
	
	addLabel(L"Initializing System...");
	CGUIEventManager * Forwarder = new CGUIEventManager(GUIManager->getCanvas(), & Application.GetWindow());
	
	addLabel(L"Loading Scene Shaders...");
	Application.GetSceneManager().init(true, true);
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

void CLoadState::Update(f32 const Elapsed)
{
	Context->GUIContext->draw(Elapsed, true);
	CApplication::Get().GetWindow().SwapBuffers();
}

void CLoadState::loadShaders()
{
	Indent = 60;
	bool Failed = false;
	
	if (! (Context->Shaders.Glyph = CShaderLoader::loadShader("Glyph")))
		addLabel(L"Failed to load Glyph Shader - Glyphs will not draw.", Gwen::Color(255, 32, 32, 192)), Failed = true;
	if (! (Context->Shaders.GlyphLines = CShaderLoader::loadShader("GlyphLines")))
		addLabel(L"Failed to load Glyph Line Shader - Glyphs Lines will not draw.", Gwen::Color(255, 32, 32, 192)), Failed = true;
	if (! (Context->Shaders.DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture")))
		addLabel(L"Failed to load Diffuse/Texture Shader - Backdrop will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Volume = CShaderLoader::loadShader("Volume")))
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
	CSceneManager * SceneManager = & CApplication::Get().GetSceneManager();

	// OpenGL Parameters
	glClearColor(0.3f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Global Light Position
	Scene.LightPosition = SVector3f(0.2f, 0.4f, 0.2f);

	// Cameras
	Scene.Camera = new CCameraControl(SVector3f(1.f, 1.3f, 4.5f));
	Scene.Camera->SetAspectRatio(CApplication::Get().GetWindow().GetAspectRatio());
	Scene.Camera->SetNearPlane(0.001f);
	Scene.Camera->SetFarPlane(100.f);
	Scene.Camera->SetVelocity(1.9f);
	CMainState::Get().IEventListener<SMouseEvent>::AddChild(Scene.Camera);
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
	SceneManager->addSceneObject(Scene.SkyBox);

	CPlaneGridSceneObject * Plane = new CPlaneGridSceneObject(10);
	Plane->setShader(SceneManager->getDefaultColorRenderPass(), Context->Shaders.GlyphLines);
	SceneManager->addSceneObject(Plane);

	// Container Objects
	Scene.GlyphSceneObject = new CGlyphSceneObject();
	SceneManager->addSceneObject(Scene.GlyphSceneObject);

	// Terrain
	Scene.Terrain = new CTerrainSceneObject();
	SceneManager->addSceneObject(Scene.Terrain);
	Scene.Terrain->setCullingEnabled(false);
	vec3f Scale = vec3f(1.f);
	Scale /= 512.f;
	Scale *= 3.f;
	Scale.X *= -1;

	// GPS Coordinates
	vec2f DataRangeMin(9.53894f, 63.59233f), DataRangeMax(9.54926f, 63.59595f);
	vec2f MapRangeMin(9.51013f, 63.57518f), MapRangeMax(9.56290f, 63.60297f);

	vec2f DataRangeCenter = (DataRangeMin + DataRangeMax) / 2.f;
	vec2f ScaleAdjust = (MapRangeMax - MapRangeMin) / (DataRangeMax - DataRangeMin);
	vec2f TranslationAdjust = (DataRangeCenter - MapRangeMin) / (MapRangeMax - DataRangeCenter);

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
	RelativeTranslate *= MaxAdjust;
	Scale.X *= MaxAdjust;
	Scale.Z *= MaxAdjust;
	Scale.Y *= (ScaleAdjust.X + ScaleAdjust.Y) / 2.f;

	Scene.Terrain->setScale(Scale);
	Scene.Terrain->setTranslation(vec3f(-RelativeTranslate.X, 0.f, RelativeTranslate.Y));

	// Volume
	Scene.VolumeSceneObject = new CVolumeSceneObject();
	//SceneManager->addSceneObject(Scene.VolumeSceneObject);

	Scene.VolumeSceneObject->setScale(Adjuster * vec3f(3.f, 1.6f, 3.f));
	Scene.GlyphSceneObject->setScale(Adjuster  * vec3f(3.f, 1.6f, 3.f) * vec3f(-1, -1, 1));

	Scene.GlyphSceneObject->setTranslation(vec3f(0, 0.4f, 0));
	Scene.VolumeSceneObject->setTranslation(vec3f(0, 0.4f, 0));
}

void CLoadState::OnFinish()
{
	// Cleanup GUI
	Canvas->RemoveAllChildren();

	Application->GetStateManager().SetState(& CMainMenuState::Get());
}
