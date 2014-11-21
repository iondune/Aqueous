
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
		CPerspectiveCamera * Camera = nullptr;
		CCameraController * CameraController = nullptr;
		ICamera * OrbitCamera = nullptr;

		// Orbit Camera Timer
		float Timer = 0;

		// Lighting
		CSceneNode * LightObject = nullptr;

		// Scene object
		CSceneNode * SkyBox = nullptr;
		SingletonPointer<CTerrainNodeManager> Terrain;
		SingletonPointer<CGlyphNodeManager> Glyphs;
		SingletonPointer<CVolumeNodeManager> Volume;
		CSceneNode * Water = nullptr;

		// Default cube mesh
		CMesh * Cube = nullptr;
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
		CShader * FXAA = nullptr;
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
	CTexture2D * FinalColor = nullptr;
	CTexture2D * AAColor = nullptr;

	string SitePath = "./Sites/";

	void Run();
	void LoadConfig();

};
