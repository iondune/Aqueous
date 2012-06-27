#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freetype.lib")

#pragma comment(lib, "matlib/libmat.lib")
#pragma comment(lib, "matlib/libmx.lib")
#pragma comment(lib, "matlib/libmex.lib")
#pragma comment(lib, "matlib/libeng.lib")
#pragma comment(lib, "matlib/libmwlapack.lib")
#pragma comment(lib, "matlib/libdflapack.lib")

#pragma comment(lib, "ionWindow.lib")
#pragma comment(lib, "ionScene.lib")
#pragma comment(lib, "ionGUI.lib")

#pragma comment(lib, "FreeImage.lib")
#ifdef _DEBUG
#pragma comment(lib, "GWEND-Renderer-SFMLd.lib")
#pragma comment(lib, "gwen_staticd.lib")
#else
#pragma comment(lib, "GWEN-Renderer-SFML.lib")
#pragma comment(lib, "gwen_static.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Debug/sfml-system-d.lib")
#pragma comment(lib, "Debug/sfml-window-d.lib")
#pragma comment(lib, "Debug/sfml-graphics-d.lib")
#pragma comment(lib, "Debug/sfml-main-d.lib")
#else
#pragma comment(lib, "Release/sfml-system.lib")
#pragma comment(lib, "Release/sfml-window.lib")
#pragma comment(lib, "Release/sfml-graphics.lib")
#endif

#endif

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CMainState.h"


int main()
{
	printf("Started...\n");
	CTextureLoader::ImageDirectory = "Media/";
	CMeshLoader::MeshDirectory = "Media/";
	CShaderLoader::ShaderDirectory = "Shaders/";

	CApplication & Application = CApplication::get();
	Application.init(SPosition2(1600, 900));

	Application.getStateManager().setState(& CMainState::get());

	Application.run();

	return 0;
}
