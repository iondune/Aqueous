#ifndef _LIBRARIES_H_INCLUDED_
#define _LIBRARIES_H_INCLUDED_

#ifdef _WIN32

// OpenGL Libraries
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glfw.lib")

// ionEngine Libraries
#pragma comment(lib, "ionWindow.lib")
#pragma comment(lib, "ionScene.lib")

// GLFW Libraries
#ifdef _DEBUG
#pragma comment(lib, "glew32sd.lib")
#else
#pragma comment(lib, "glew32s.lib")
#endif

// Rocket
#pragma comment(lib, "RocketCore.lib")
#pragma comment(lib, "RocketDebugger.lib")
#pragma comment(lib, "RocketControls.lib")

// MatLAB Libraries
#pragma comment(lib, "matlib/libmat.lib")
#pragma comment(lib, "matlib/libmx.lib")
#pragma comment(lib, "matlib/libmex.lib")
#pragma comment(lib, "matlib/libeng.lib")
#pragma comment(lib, "matlib/libmwlapack.lib")
#pragma comment(lib, "matlib/libdflapack.lib")

#endif

#endif
