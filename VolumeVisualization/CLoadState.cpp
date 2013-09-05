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

enum class ECompassDirection
{
	N = 1, E = 1,
	S = -1, W = -1
};

static f64 LongLatDecimalDegrees(f64 const Deg, f64 const Min, f64 const Sec, ECompassDirection const Direction = ECompassDirection::N)
{
	return (Deg + Min/60.0 + Sec/3600.0) * (int) Direction;
}

static f64 LongLatDecimalDegrees(std::string const & String)
{
	f64 Deg, Min, Sec;
	char Dir, Dummy;
	ECompassDirection Direction;

	sscanf(String.c_str(), "%lf %c %lf %c %lf %c %c", & Deg, & Dummy, & Min, & Dummy, & Sec, & Dummy, & Dir);
	
	switch (tolower(Dir))
	{
	default:
	case 'N':
		Direction = ECompassDirection::N;
		break;
	case 'E':
		Direction = ECompassDirection::E;
		break;
	case 'S':
		Direction = ECompassDirection::S;
		break;
	case 'W':
		Direction = ECompassDirection::W;
		break;
	}

	return LongLatDecimalDegrees(Deg, Min, Sec, Direction);

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
	Plane->setVisible(false);
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

	// Volume
	Scene.Volume = new CVolumeSceneObject();
	Scene.Volume->setVisible(false);
	//SceneManager->addSceneObject(Scene.VolumeSceneObject);

	// Coordinate Calculations
	vec2f const DataRangeMin(56.667337f, 9.988503f), DataRangeMax(56.673766f, 9.999437f);
	vec2f const MapRangeMin(LongLatDecimalDegrees(56, 38, 17.22), LongLatDecimalDegrees(9, 55, 45.32)), MapRangeMax(LongLatDecimalDegrees(56, 41, 59.01), LongLatDecimalDegrees(10, 2, 34.80));
	
	vec2f const DataRangeSize = DataRangeMax - DataRangeMin;
	vec2f const DataRangeCenter = (DataRangeMin + DataRangeMax) / 2.f;
	vec2f const DataActualSize(DistFrom(DataRangeMin.X, DataRangeCenter.Y, DataRangeMax.X, DataRangeCenter.Y), DistFrom(DataRangeCenter.X, DataRangeMin.Y, DataRangeCenter.X, DataRangeMax.Y));
	f32 const DataDepth = 15.08980f;
	
	vec2f const MapRangeSize = MapRangeMax - MapRangeMin;
	vec2f const MapRangeCenter = (MapRangeMin + MapRangeMax) / 2.f;
	vec2f const MapActualSize(DistFrom(MapRangeMin.X, MapRangeCenter.Y, MapRangeMax.X, MapRangeCenter.Y), DistFrom(MapRangeCenter.X, MapRangeMin.Y, MapRangeCenter.X, MapRangeMax.Y));
	f32 const MapDepth = 600.f;

	printf("Data range is %f by %f meters,\n", DataActualSize.X, DataActualSize.Y);
	
	vec2f const ActualOffset = vec2f(DistFrom(DataRangeCenter.X, DataRangeCenter.Y, MapRangeCenter.X, DataRangeCenter.Y), DistFrom(DataRangeCenter.X, DataRangeCenter.Y, DataRangeCenter.X, MapRangeCenter.Y));
	vec2f const MapOffset = ActualOffset * 3.f / Maximum(DataActualSize.X, DataActualSize.Y);
	vec3f const DataScale = 3.f * vec3f(DataActualSize.X, DataDepth, DataActualSize.Y) / Maximum(DataActualSize.X, DataActualSize.Y);
	vec3f const MapScale = DataScale * vec3f(MapActualSize.X, MapDepth, MapActualSize.Y) / vec3f(DataActualSize.X, DataDepth, DataActualSize.Y);

	static f32 const YExaggeration = 3.f;
	static vec3f const Multiplier = vec3f(1, YExaggeration, 1);
	
	Scene.Glyphs->setScale(DataScale * Multiplier);
	Scene.Glyphs->setTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, 0));

	Scene.Terrain->setScale(MapScale * Multiplier / CTerrainSceneObject::Size);
	Scene.Water->setScale(MapScale / CTerrainSceneObject::Size);

	Scene.Terrain->setTranslation(vec3f(MapOffset.X, 0, MapOffset.Y));
	Scene.Water->setTranslation(vec3f(MapOffset.X, 0, MapOffset.Y));
}

void CLoadState::OnFinish()
{
	// Cleanup GUI
	Canvas->RemoveAllChildren();

	Application->GetStateManager().SetState(& CMainMenuState::Get());
}
