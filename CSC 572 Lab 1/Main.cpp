#include "Libraries.h"

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CMainState.h"
#include "ContourConverter.h"

int main()
{
	std::cout << "System is initializing..." << std::endl;

	// Directory Setup
	CTextureLoader::ImageDirectory = "Media/";
	CMeshLoader::MeshDirectory = "Media/";
	CShaderLoader::ShaderDirectory = "Shaders/";

	// Option to run contour converter
	if (false)
	{
		ContourConverter c;
		c.convert();
	}
	else
	{
		// Create Window
		CApplication & Application = CApplication::get();
		Application.init(SPosition2(1600, 900), "Underwater Volume Data Rendering");

		std::cout << "GUI Engine is initializing..." << std::endl;

		CMainState & MainState = CMainState::get();
		MainState.loadGUIEngine();
		MainState.startLoadingContext();

		Application.loadEngines();
		MainState.loadEngineReferences();

		// Load custom state
		Application.getStateManager().setState(& CMainState::get());

		// Run program!
		Application.run();
	}

	return 0;
}
