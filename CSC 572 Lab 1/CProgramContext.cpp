#include "CProgramContext.h"

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CMainState.h"
#include "CLoadState.h"


void CProgramContext::init()
{
	// Create Window
	CApplication & Application = CApplication::get();
	Application.init(SPosition2(1600, 900), "Underwater Volume Data Rendering");
	CMainState & MainState = CMainState::get();

	// Create GUI Engine
	std::cout << "GUI Engine is initializing..." << std::endl;
	GUIManager = new CGUIManager();
	GUIManager->init();

	// Begin loading
	MainState.startLoadingContext();

	Application.loadEngines();
	MainState.loadEngineReferences();
}

void CProgramContext::run()
{
	CApplication & Application = CApplication::get();

	// Load initial state
	Application.getStateManager().setState(& CMainState::get());

	// Run program!
	Application.run();
}