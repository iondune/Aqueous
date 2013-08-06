#pragma once

// OpenGL Libraries
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glfw3.lib")

#pragma comment(lib, "FreeImage.lib")

// ionEngine Libraries
#pragma comment(lib, "ionWindow.lib")
#pragma comment(lib, "ionScene.lib")
#pragma comment(lib, "ionGUI.lib")

// GLEW Libraries
#ifdef _DEBUG
#pragma comment(lib, "glew32sd.lib")
#else
#pragma comment(lib, "glew32s.lib")
#endif

// GWEN Libraries
#ifdef _DEBUG
#pragma comment(lib, "GWEND-Renderer-OpenGLd.lib")
#pragma comment(lib, "gwen_staticd.lib")
#else
#pragma comment(lib, "GWEN-Renderer-OpenGL.lib")
#pragma comment(lib, "gwen_static.lib")
#endif

// freetype
#ifdef _DEBUG
#pragma comment(lib, "freetype250_D.lib")
#else
#pragma comment(lib, "freetype250.lib")
#endif

// MatLAB Libraries
#pragma comment(lib, "matlib/libmat.lib")
#pragma comment(lib, "matlib/libmx.lib")
#pragma comment(lib, "matlib/libmex.lib")
#pragma comment(lib, "matlib/libeng.lib")
#pragma comment(lib, "matlib/libmwlapack.lib")
#pragma comment(lib, "matlib/libdflapack.lib")
