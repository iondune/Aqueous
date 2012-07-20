#ifdef _WIN32

// OpenGL Libraries
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

// ionEngine Libraries
#ifdef _DEBUG
#pragma comment(lib, "Debug/ionWindow.lib")
#pragma comment(lib, "Debug/ionScene.lib")
#pragma comment(lib, "Debug/ionGUI.lib")
#else
#pragma comment(lib, "Release/ionWindow.lib")
#pragma comment(lib, "Release/ionScene.lib")
#pragma comment(lib, "Release/ionGUI.lib")
#endif

// SFML Libraries
#ifdef _DEBUG
#pragma comment(lib, "sfml-system-s-d.lib")
#pragma comment(lib, "sfml-window-s-d.lib")
#pragma comment(lib, "sfml-graphics-s-d.lib")
#else
#pragma comment(lib, "sfml-system-s.lib")
#pragma comment(lib, "sfml-window-s.lib")
#pragma comment(lib, "sfml-graphics-s.lib")
#endif

// GWEN Libraries
#pragma comment(lib, "FreeImage.lib")
#ifdef _DEBUG
#pragma comment(lib, "GWENd-Renderer-SFML.lib")
#pragma comment(lib, "gwend_static.lib")
#else
#pragma comment(lib, "GWEN-Renderer-SFML.lib")
#pragma comment(lib, "gwen_static.lib")
#endif

// MatLAB Libraries
#pragma comment(lib, "matlib/libmat.lib")
#pragma comment(lib, "matlib/libmx.lib")
#pragma comment(lib, "matlib/libmex.lib")
#pragma comment(lib, "matlib/libeng.lib")
#pragma comment(lib, "matlib/libmwlapack.lib")
#pragma comment(lib, "matlib/libdflapack.lib")

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
	Application.init(SPosition2(1600, 900), "Underwater Volume Data Rendering");

	Application.getStateManager().setState(& CMainState::get());

	Application.run();

	return 0;
}
