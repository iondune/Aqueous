#include "CProgramContext.h"

#include "CMainState.h"
#include "CLoadContext.h"


CProgramContext::SScene::SScene()
	: Camera(0), OrbitCamera(0), Timer(0.f),
	LightObject(0), SkyBox(0), Terrain(0)
{}

CProgramContext::SShaders::SShaders()
	: Diffuse(0), DiffuseTexture(0), Volume(0), Terrain(0)
{}

CProgramContext::CProgramContext()
	: GUIContext(0), DataManager(0)
{}

void CProgramContext::init()
{
	// Create Window
	CApplication & Application = CApplication::get();
	Application.init(SPosition2(1024, 768), "Underwater Volume Data Rendering");
	CMainState & MainState = CMainState::get();

	// Create GUI Engine
	std::cout << "GUI Engine is initializing..." << std::endl;
	GUIContext = new CGUIContext();
	GUIContext->init();

	// Begin loading
	CLoadContext LoadContext;
	LoadContext.init();
	LoadContext.run();
}

void CProgramContext::run()
{
	CApplication & Application = CApplication::get();

	// Load initial state
	Application.getStateManager().setState(& CMainState::get());

	// Run program!
	Application.run();
}
