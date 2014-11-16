
#include "CLoadState.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CSite.h"

#include "CTerrainNodeManager.h"
#include "CVolumeNodeManager.h"
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
	Context->Window->SwapBuffers();

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
	SingletonPointer<CMainState> MainState;
	SingletonPointer<CMainMenuState> MenuState;

	GUIManager = Context->GUIContext->Manager.Get();
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

	GUIManager->Draw(0, true);
	Context->Window->SwapBuffers();
	
	AddLabel(L"Initializing System...");
	CGUIEventManager * Forwarder = new CGUIEventManager(GUIManager->GetCanvas(), Context->Window);
	
	AddLabel(L"Loading Scene Shaders...");
	//SceneManager->init(true, true);
	LoadShaders();
	
	AddLabel(L"Loading Scene Objects...");
	LoadScene();
	
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
	GUIManager->Draw(Elapsed, true);
}

void CLoadState::LoadShaders()
{
	Indent = 60;
	bool Failed = false;
	
	if (! (Context->Shaders.Glyph = SceneManager->GetShaderLibrary()->Load("Glyph")))
		AddLabel(L"Failed to load Glyph Shader - Glyphs will not draw.", Gwen::Color(255, 32, 32, 192)), Failed = true;
	if (! (Context->Shaders.GlyphLines = SceneManager->GetShaderLibrary()->Load("GlyphLines")))
		AddLabel(L"Failed to load Glyph Line Shader - Glyphs Lines will not draw.", Gwen::Color(255, 32, 32, 192)), Failed = true;
	if (! (Context->Shaders.DiffuseTexture = SceneManager->GetShaderLibrary()->Load("DiffuseTexture")))
		AddLabel(L"Failed to load Diffuse/Texture Shader - Backdrop will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Volume = SceneManager->GetShaderLibrary()->Load("Volume")))
		AddLabel(L"Failed to load Volume Shader - Volume will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Terrain = SceneManager->GetShaderLibrary()->Load("Terrain")))
		AddLabel(L"Failed to load Terrain Shader - Terrain will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Plane = SceneManager->GetShaderLibrary()->Load("Plane")))
		AddLabel(L"Failed to load Plane Shader - Plane will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Water = SceneManager->GetShaderLibrary()->Load("Water")))
		AddLabel(L"Failed to load Water Shader - Water will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.SkyBox = SceneManager->GetShaderLibrary()->Load("Skybox")))
		AddLabel(L"Failed to load Skybox Shader - Skybox will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Merge = SceneManager->GetShaderLibrary()->Load("Merge")))
		AddLabel(L"Failed to load Merge Shader - Water surface will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.Refract = SceneManager->GetShaderLibrary()->Load("Refract")))
		AddLabel(L"Failed to load Refract Shader - Water surface  will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;
	if (! (Context->Shaders.White = SceneManager->GetShaderLibrary()->Load("White")))
		AddLabel(L"Failed to load White Shader - Water surface  will not draw.", Gwen::Color(255, 64, 64, 192)), Failed = true;

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

	// Render Pass Setup
	CRenderPassManager * RenderPassManager = SceneManager->GetRenderPassManager();

	CFrameBuffer * DefaultFrameBuffer = new CFrameBuffer{};
	Context->SceneColorTexture = DefaultFrameBuffer->MakeScreenSizedColorAttachment(0);
	Context->SceneDepthBuffer = DefaultFrameBuffer->MakeScreenSizedDepthTextureAttachment();
	RenderPassManager->AddRenderPass("Default", DefaultFrameBuffer);

	CFrameBuffer * VolumeFrameBuffer = new CFrameBuffer{};
	VolumeFrameBuffer->AttachColorTexture(DefaultFrameBuffer->GetColorTextureAttachment(0), 0);
	RenderPassManager->AddRenderPass("Volume", VolumeFrameBuffer)->SetClearBuffers({});

	CFrameBuffer * MaskFrameBuffer = new CFrameBuffer();
	CTexture2D * MaskTexture = Context->RefractMaskTexture = MaskFrameBuffer->MakeScreenSizedColorAttachment(0);
	MaskFrameBuffer->AttachDepthTexture(Context->SceneDepthBuffer);
	RenderPassManager->AddRenderPass("RefractionMask", MaskFrameBuffer)->SetClearBuffers({ion::GL::EBuffer::Color});

	CFrameBuffer * RefractFrameBuffer = new CFrameBuffer();
	Context->SceneRefractColor = RefractFrameBuffer->MakeScreenSizedColorAttachment(0);
	RefractFrameBuffer->AttachDepthTexture(Context->SceneDepthBuffer);
	RenderPassManager->AddRenderPass("Refraction", RefractFrameBuffer)->SetClearBuffers({ion::GL::EBuffer::Color});

	RenderPassManager->SetRenderPassOrder({"Default", "Volume", "RefractionMask", "Refraction"});

	// Cameras
	Scene.Camera = SceneManager->GetFactory()->AddPerspectiveCamera(Context->Window->GetAspectRatio());
	Scene.CameraController = new CCameraController(Scene.Camera);//SVector3f(1.f, 1.3f, 4.5f));
	Scene.CameraController->SetVelocity(1.9f);
	Context->Window->AddListener(Scene.CameraController);

	Scene.OrbitCamera = SceneManager->GetFactory()->AddPerspectiveCamera(Context->Window->GetAspectRatio());
	SceneManager->GetScene()->SetActiveCamera(Scene.Camera);

	// Basic Shader/Mesh
	SceneManager->GetMeshLibrary()->Add("Cube", CGeometryCreator::CreateCube());
	SceneManager->GetMeshLibrary()->Add("Sphere", CGeometryCreator::CreateSphere());
	SceneManager->GetMeshLibrary()->Add("Plane", CGeometryCreator::CreatePlane());
	
	SceneManager->GetTextureLibrary()->Load("SkyMap.jpg");
	SceneManager->GetTextureLibrary()->Load("WaterNormals.jpg");

	// Backdrop
	Scene.SkyBox = SceneManager->GetFactory()->AddSkySphereNode("SkyMap.jpg");

	//CPlaneGridSceneObject * Plane = new CPlaneGridSceneObject(10);
	//Plane->setShader(SceneManager->getDefaultColorRenderPass(), Context->Shaders.Plane);
	//Plane->setVisible(false);
	//SceneManager->addSceneObject(Plane);

	// Container Objects
	//Scene.Glyphs = new CGlyphSceneObject();
	//SceneManager->addSceneObject(Scene.Glyphs);

	// Terrain
	Scene.Terrain->Load();

	// Volume
	Scene.Volume->Load();

	// Water
	Scene.Water = SceneManager->GetFactory()->AddSceneNode();
	Scene.Water->SetDebugName("Water");
	Scene.Water->SetMesh(SceneManager->GetMeshLibrary()->Get("Plane"));
	Scene.Water->SetShader(SceneManager->GetShaderLibrary()->Get("Refract"), "Refraction");
	Scene.Water->SetShader(SceneManager->GetShaderLibrary()->Get("White"), "RefractionMask");
	Scene.Water->SetTexture(0, SceneManager->GetTextureLibrary()->Get("WaterNormals.jpg"));
	Scene.Water->SetTexture(1, Context->SceneColorTexture);
	Scene.Water->SetTexture(2, MaskTexture);
	Scene.Water->SetScale(vec3f(25.f));
	Scene.Water->SetFeatureEnabled(ion::GL::EDrawFeature::DisableDepthTest, "Refraction", true);
}

void CLoadState::OnFinish()
{
	// Cleanup GUI
	Canvas->RemoveAllChildren();

	StateManager->SetState(CMainMenuState::GetPtr());
}
