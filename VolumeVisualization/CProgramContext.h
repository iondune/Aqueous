
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>
#include <ionFramework.h>
#include <ionScience.h>
#include <ionBootstrap.h>

#include "CGUIContext.h"
#include "CContextState.h"

class CTerrainNodeManager;
class CVolumeSceneObject;
class CGlyphSceneObject;
class CWaterSceneObject;

class CSite;


class CProgramContext : public Singleton<CProgramContext>
{

public:

	//! Hold scene parameters
	struct SScene
	{
		// Cameras
		CPerspectiveCamera * Camera = 0;
		CCameraController * CameraController = 0;
		ICamera * OrbitCamera = 0;

		// Orbit Camera Timer
		float Timer = 0;

		// Lighting
		CSceneNode * LightObject = 0;
		SVector3f LightPosition = 0;

		// Scene object
		CSceneNode * SkyBox = 0;
		SingletonPointer<CTerrainNodeManager> Terrain;
		CGlyphSceneObject * Glyphs = 0;
		CVolumeSceneObject * Volume = 0;
		CWaterSceneObject * Water = 0;

		// Default cube mesh
		CMesh * Cube = 0;
	};

	//! Holds shaders
	struct SShaders
	{
		CShader * Diffuse = 0;
		CShader * DiffuseTexture = 0;
		CShader * Terrain = 0;
		CShader * Volume = 0;
		CShader * Glyph = 0;
		CShader * GlyphLines = 0;
		CShader * Plane = 0;
		CShader * Water = 0;
		CShader * SkyBox = 0;
	};

	SShaders Shaders;
	SScene Scene;

	SingletonPointer<CGUIContext> GUIContext;
	CSite * CurrentSite = 0;
	CWindow * Window = 0;

	void Run();

};
