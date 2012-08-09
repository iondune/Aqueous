#include "CProgramContext.h"

#include "CMainState.h"
#include "CLoadContext.h"


CProgramContext::CProgramContext()
	: GUIManager(0)
{
	DataParser[0] = DataParser[1] = DataParser[2] = 0;
}

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
