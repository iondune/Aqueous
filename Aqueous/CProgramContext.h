
#pragma once

#include <ionEngine.h>
#include "CContextState.h"
#include "CGUIContext.h"


class CGUIContext;

class CTerrainNodeManager;
class CVolumeNodeManager;
class CGlyphNodeManager;

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

		// Scene object
		CSceneNode * SkyBox = 0;
		SingletonPointer<CTerrainNodeManager> Terrain;
		SingletonPointer<CGlyphNodeManager> Glyphs;
		SingletonPointer<CVolumeNodeManager> Volume;
		CSceneNode * Water = 0;

		// Default cube mesh
		CMesh * Cube = 0;
	};

	//! Holds shaders
	struct SShaders
	{
		CShader * Diffuse = nullptr;
		CShader * DiffuseTexture = nullptr;
		CShader * Terrain = nullptr;
		CShader * Volume = nullptr;
		CShader * Glyph = nullptr;
		CShader * Water = nullptr;
		CShader * Merge = nullptr;
		CShader * Refract = nullptr;
		CShader * White = nullptr;
	};

	SShaders Shaders;
	SScene Scene;

	SingletonPointer<CGUIContext> GUIContext;
	CSite * CurrentSite = 0;
	CWindow * Window = 0;
	CTexture2D * SceneColorTexture = nullptr;
	CTexture2D * SceneDepthBuffer = nullptr;
	CTexture2D * SceneRefractColor = nullptr;
	CTexture2D * RefractMaskTexture = nullptr;

	string SitePath = "./Sites/";

	void Run();
	void LoadConfig();

};
