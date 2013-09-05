#include "CLoadState.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CGUIEventManager.h"

#include "SciDataManager.h"
#include "CTerrainSceneObject.h"
#include "CWaterSceneObject.h"
#include "CVolumeSceneObject.h"
#include "CGlyphSceneObject.h"
#include "CPlaneGridSceneObject.h"


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
	
	Context->DataManager = new SciDataManager();
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


static double const pi = 3.14159;
static double const toRadians(double const deg)
{
	return deg * pi / 180;
}

static double DistFrom(double lat1, double lng1, double lat2, double lng2)
{
	double earthRadius = 3958.75;
	double dLat = toRadians(lat2-lat1);
	double dLng = toRadians(lng2-lng1);
	double a = sin(dLat/2) * sin(dLat/2) +
		cos(toRadians(lat1)) * cos(toRadians(lat2)) *
		sin(dLng/2) * sin(dLng/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double dist = earthRadius * c;

	int meterConversion = 1609;

	return (dist * meterConversion);
}

void CLoadState::LoadScene()
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

	// Backdrop
	Scene.SkyBox = new CMeshSceneObject();
	Scene.SkyBox->setMesh(Scene.Cube);
	Scene.SkyBox->setShader(SceneManager->getDefaultColorRenderPass(), Context->Shaders.DiffuseTexture);
	Scene.SkyBox->setScale(SVector3f(40.f));
	Scene.SkyBox->setTexture(0, "Space.bmp");
	Scene.SkyBox->setCullingEnabled(false);
	Scene.SkyBox->setVisible(false);
	SceneManager->addSceneObject(Scene.SkyBox);

	CPlaneGridSceneObject * Plane = new CPlaneGridSceneObject(10);
	Plane->setShader(SceneManager->getDefaultColorRenderPass(), Context->Shaders.Plane);
	SceneManager->addSceneObject(Plane);

	// Container Objects
	Scene.Glyphs = new CGlyphSceneObject();
	SceneManager->addSceneObject(Scene.Glyphs);

	// Water
	Scene.Water = new CWaterSceneObject();
	//SceneManager->addSceneObject(Scene.Water);

	// Terrain
	Scene.Terrain = new CTerrainSceneObject();
	SceneManager->addSceneObject(Scene.Terrain);

	// GPS Coordinates
	vec2f DataRangeMin(63.59233f, 9.53894f), DataRangeMax(63.59595f, 9.54926f);
	vec2f MapRangeMin(63.57518f, 9.51013f), MapRangeMax(63.60297f, 9.56290f);

	printf("Data range is %f by %f meters,\n",
		DistFrom(DataRangeMin.X, DataRangeMin.Y, DataRangeMax.X, DataRangeMin.Y),
		DistFrom(DataRangeMin.X, DataRangeMin.Y, DataRangeMin.X, DataRangeMax.Y));

	vec2f DataRangeCenter = (DataRangeMin + DataRangeMax) / 2.f;

	// Ratio of map range size to data range size
	vec2f ScaleAdjust = (MapRangeMax - MapRangeMin) / (DataRangeMax - DataRangeMin);
	f32 MaxAdjust = Maximum(ScaleAdjust.X, ScaleAdjust.Y);

	// Ratio of lower-side offset to upper-side offset
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

	vec3f Adjuster(1.f);
	if (ScaleAdjust.X > ScaleAdjust.Y)
		Adjuster.Z = ScaleAdjust.X / ScaleAdjust.Y;
	else
		Adjuster.X = ScaleAdjust.Y / ScaleAdjust.X;

	RelativeTranslate *= MaxAdjust;

	vec3f Scale = vec3f(1.f);
	Scale /= 512.f;
	Scale *= 3.f;
	Scale.X *= -1;
	Scale.X *= MaxAdjust;
	Scale.Z *= MaxAdjust;
	Scale.Y *= (ScaleAdjust.X + ScaleAdjust.Y) / 2.f;
	
	Scene.Terrain->setScale(Scale);
	Scene.Terrain->setTranslation(vec3f(-RelativeTranslate.X, 0.f, RelativeTranslate.Y));
	Scene.Water->setScale(Scale);
	Scene.Water->setTranslation(vec3f(-RelativeTranslate.X, 0.f, RelativeTranslate.Y));

	// Volume
	Scene.Volume = new CVolumeSceneObject();
	//SceneManager->addSceneObject(Scene.VolumeSceneObject);

	Scene.Volume->setScale(Adjuster * vec3f(3.f, 1.6f, 3.f));
	Scene.Glyphs->setScale(Adjuster * vec3f(3.f, 1.6f, 3.f) * vec3f(-1, -1, 1));

	Scene.Volume->setTranslation(vec3f(0, 0.4f - 1.f, 0));
	Scene.Glyphs->setTranslation(vec3f(0, 0.4f - 1.f, 0));
}

void CLoadState::OnFinish()
{
	// Cleanup GUI
	Canvas->RemoveAllChildren();

	Application->GetStateManager().SetState(& CMainMenuState::Get());
}
