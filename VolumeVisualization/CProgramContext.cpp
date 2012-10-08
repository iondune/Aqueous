#include "CProgramContext.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CLoadState.h"


CProgramContext::SScene::SScene()
	: Camera(0), OrbitCamera(0), Timer(0.f),
	LightObject(0), SkyBox(0), Terrain(0), GlyphSceneObject(0), 
	VolumeSceneObject(0), Cube(0)
{}

CProgramContext::SShaders::SShaders()
	: Diffuse(0), DiffuseTexture(0), Volume(0), Terrain(0), Glyph(0)
{}

CProgramContext::CProgramContext()
	: GUIContext(0), DataManager(0)
{}

void CProgramContext::run()
{
	// Create Window
	CApplication & Application = CApplication::get();
	Application.init(SPosition2(1600, 900), "Underwater Volume Data Rendering");

	Application.loadEngines();

	// Create GUI Engine
	std::cout << "GUI Engine is initializing..." << std::endl;
	GUIContext = new CGUIContext();
	GUIContext->init();

	// Begin loading
	CLoadState & LoadState = CLoadState::get();
	Application.getStateManager().setState(& LoadState);

	// Run program
	Application.run();
}
