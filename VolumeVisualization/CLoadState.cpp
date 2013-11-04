
#include "CLoadState.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CSite.h"

#include "CTerrainSceneObject.h"
#include "CWaterSceneObject.h"
#include "CVolumeSceneObject.h"
#include "CGlyphSceneObject.h"
#include "CPlaneGridSceneObject.h"

#include <ionGUI/CGUIEventManager.h>


void CLoadStateEventHandler::OnFinish(Gwen::Controls::Base * Control)
{
	CLoadState::Get().OnFinish();
}

void CLoadState::AddLabel(std::wstring const & Label, Gwen::Color const & Color)
{
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Canvas);
	MediumLabel->SetFont(GUIManager->GetMediumFont());
	MediumLabel->SetText(Label);
	MediumLabel->SetBounds(20 + Indent, LabelHeight, 1024, 300);
	MediumLabel->SetTextColor(Color);

	GUIManager->Draw(true);
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
	Canvas = GUIManager->GetCanvas();
	
	// Init Canvas
	Canvas->SetBackgroundColor(Gwen::Color(32, 48, 48));
	Canvas->SetDrawBackground(true);

	// Top Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(Canvas);
	BigLabel->SetFont(GUIManager->GetLargeFont());
	BigLabel->SetText(L"Loading...");
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(255, 255, 255, 84));

	GUIManager->Draw(true);
	CApplication::Get().GetWindow().SwapBuffers();
	
	AddLabel(L"Initializing System...");
	CGUIEventManager * Forwarder = new CGUIEventManager(GUIManager->GetCanvas(), & Application.GetWindow());
	
	AddLabel(L"Loading Scene Shaders...");
	Application.GetSceneManager().init(true, true);
	LoadShaders();
	
	AddLabel(L"Loading Scene Objects...");
	LoadScene();
	
	Context->CurrentSite = new CSite();
	AddLabel(L"Menu is Starting...");

	if (GetConfirmation)
	{
		Gwen::Controls::Button * Button = new Gwen::Controls::Button(GUIManager->GetCanvas());
		Button->SetBounds(250, 650, 250, 35);
		Button->SetText(L"Continue");
		Button->onPress.Add(& Handler, & CLoadStateEventHandler::OnFinish);
	}
	else
	{
		OnFinish();
	}
}

void CLoadState::Update(f32 const Elapsed)
{
	Context->GUIContext->Draw(Elapsed, true);
	CApplication::Get().GetWindow().SwapBuffers();
}

void CLoadState::LoadShaders()
{
	Indent = 60;
	bool Failed = false;
	
	if (! (Context->Shaders.Glyph = CShaderLoader::loadShader("Glyph")))
		AddLabel(L"Failed to load Glyph Shader - Glyphs will not draw.", Gwen::Color(255, 32, 32, 192)), Failed = true;
	if (! (Context->Shaders.GlyphLines = CShaderLoader::loadShader("GlyphLines")))
		AddLabel(L"Failed to load Glyph Line Shader - Glyphs Lines will not draw.", Gwen::Color(255, 32, 32, 192)), Failed = true;
	if (! (Context->Shaders.DiffuseTexture = CShaderLoader::loadShader("DiffuseTexture")))
		AddLabel(L"Failed to load Diffuse/Texture Shader - Backdrop will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Volume = CShaderLoader::loadShader("Volume")))
		AddLabel(L"Failed to load Volume Shader - Volume will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Terrain = CShaderLoader::loadShader("Terrain")))
		AddLabel(L"Failed to load Terrain Shader - Terrain will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Plane = CShaderLoader::loadShader("Plane")))
		AddLabel(L"Failed to load Plane Shader - Plane will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Water = CShaderLoader::loadShader("Water")))
		AddLabel(L"Failed to load Water Shader - Water will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;

	if (! Failed)
		AddLabel(L"All shaders compiled successfully.", Gwen::Color(64, 255, 64, 192));
	else
		GetConfirmation = true;

	Indent = 0;
}

void CLoadState::LoadScene()
{
	// References
	CProgramContext::SScene & Scene = Context->Scene;
	CSceneManager * SceneManager = & CApplication::Get().GetSceneManager();

	// OpenGL Parameters
	glClearColor(0.15f, 0.45f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Global Light Position
	Scene.LightPosition = SVector3f(0.2f, 0.4f, 0.2f);

	// Cameras
	Scene.Camera = new CCameraControl(SVector3f(1.f, 1.3f, 4.5f));
	Scene.Camera->SetAspectRatio(CApplication::Get().GetWindow().GetAspectRatio());
	Scene.Camera->UpdateProjection();
	Scene.Camera->SetVelocity(1.9f);
	CMainState::Get().IEventListener<SMouseEvent>::AddChild(Scene.Camera);

	Scene.OrbitCamera = new CPerspectiveCameraSceneObject();
	SceneManager->setActiveCamera(Scene.Camera);

	// Basic Shader/Mesh
	Scene.Cube = CMeshLoader::createCubeMesh();

	// Backdrop
	Scene.SkyBox = new CMeshSceneObject();
	Scene.SkyBox->setMesh(Scene.Cube);
	Scene.SkyBox->setShader(SceneManager->getDefaultColorRenderPass(), Context->Shaders.DiffuseTexture);
	Scene.SkyBox->setTexture(0, "Space.bmp");
	Scene.SkyBox->setCullingEnabled(false);
	Scene.SkyBox->setVisible(false);
	SceneManager->addSceneObject(Scene.SkyBox);

	CPlaneGridSceneObject * Plane = new CPlaneGridSceneObject(10);
	Plane->setShader(SceneManager->getDefaultColorRenderPass(), Context->Shaders.Plane);
	Plane->setVisible(false);
	SceneManager->addSceneObject(Plane);

	// Container Objects
	Scene.Glyphs = new CGlyphSceneObject();
	SceneManager->addSceneObject(Scene.Glyphs);

	// Terrain
	Scene.Terrain = new CTerrainSceneObject();
	SceneManager->addSceneObject(Scene.Terrain);

	// Water
	Scene.Water = new CWaterSceneObject();
	//SceneManager->addSceneObject(Scene.Water);

	// Volume
	Scene.Volume = new CVolumeSceneObject();
	Scene.Volume->setVisible(false);
	//SceneManager->addSceneObject(Scene.VolumeSceneObject);
}

void CLoadState::OnFinish()
{
	// Cleanup GUI
	Canvas->RemoveAllChildren();

	Application->GetStateManager().SetState(& CMainMenuState::Get());
}
