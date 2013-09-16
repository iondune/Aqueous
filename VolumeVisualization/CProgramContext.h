
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>
#include <ionFramework.h>
#include <ionScience.h>

#include "CGUIContext.h"


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
		CCameraControl * Camera;
		ICameraSceneObject * OrbitCamera;

		// Orbit Camera Timer
		float Timer;

		// Lighting
		CMeshSceneObject * LightObject;
		SVector3f LightPosition;

		// Scene object
		CMeshSceneObject * SkyBox;
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

		SShaders();
	};

	SShaders Shaders;
	SScene Scene;

	CGUIContext * GUIContext;
	CSite * CurrentSite;

	CProgramContext();
	void Run();

};
