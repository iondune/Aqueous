#ifndef _LIBRARIES_H_INCLUDED_
#define _LIBRARIES_H_INCLUDED_

#ifdef _WIN32

// OpenGL Libraries
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew.lib")

// ionEngine Libraries
#pragma comment(lib, "ionWindow.lib")
#pragma comment(lib, "ionScene.lib")

// SFML Libraries
#ifdef _DEBUG
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-graphics.lib")
#endif

// GWEN Libraries
#ifdef _DEBUG
#pragma comment(lib, "GWENd-Renderer-SFMLd.lib")
#pragma comment(lib, "gwen_staticd.lib")
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

#endif
