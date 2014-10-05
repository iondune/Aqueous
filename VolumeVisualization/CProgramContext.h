
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>
#include <ionFramework.h>
#include <ionScience.h>
#include <ionBootstrap.h>

#include "CGUIContext.h"
#include "CContextState.h"

class CTerrainSceneObject;
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
		CPerspectiveCamera * Camera;
		CCameraController * CameraController;
		ICamera * OrbitCamera;

		// Orbit Camera Timer
		float Timer;

		// Lighting
		CSceneNode * LightObject;
		SVector3f LightPosition;

		// Scene object
		CSceneNode * SkyBox;
		CTerrainSceneObject * Terrain;
		CGlyphSceneObject * Glyphs;
		CVolumeSceneObject * Volume;
		CWaterSceneObject * Water;

		// Default cube mesh
		CMesh * Cube;

		SScene();
	};

	//! Holds shaders
	struct SShaders
	{
		CShader * Diffuse;
		CShader * DiffuseTexture;
		CShader * Terrain;
		CShader * Volume;
		CShader * Glyph;
		CShader * GlyphLines;
		CShader * Plane;
		CShader * Water;
		CShader * SkyBox;

		SShaders();
	};

	SShaders Shaders;
	SScene Scene;

	CGUIContext * GUIContext;
	CSite * CurrentSite;
	CWindow * Window;

	CProgramContext();
	void Run();

};
