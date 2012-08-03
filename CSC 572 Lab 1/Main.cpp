#include "Libraries.h"

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CMainState.h"
#include "ContourConverter.h"

int main()
{
	// Directory Setup
	CTextureLoader::ImageDirectory = "Media/";
	CMeshLoader::MeshDirectory = "Media/";
	CShaderLoader::ShaderDirectory = "Shaders/";

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

		// Load custom state
		Application.getStateManager().setState(& CMainState::get());

		// Run program!
		Application.run();
	}

	return 0;
}
