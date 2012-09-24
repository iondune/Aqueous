#ifndef _CPROGRAMCONTEXT_H_INCLUDED_
#define _CPROGRAMCONTEXT_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CGUIContext.h"


class CTerrainSceneObject;
class CVolumeSceneObject;

class SciDataManager;


class CProgramContext : public Singleton<CProgramContext>
{

public:

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

		// Skybox object
		CMeshSceneObject * SkyBox;
		
		// Terrain
		CTerrainSceneObject * Terrain;
		
		// Holders for point cloud and grid glyph representations
		ISceneObject * PointCloudObject;
		ISceneObject * GridObject;
	
		// Volume
		CVolumeSceneObject * VolumeSceneObject;

		// Default cube mesh
		CMesh * Cube;


		SScene();
	};

	SScene Scene;
	CGUIContext * GUIContext;
	SciDataManager * DataManager;

	struct SShaders
	{
		CShader * Diffuse;
		CShader * DiffuseTexture;
		CShader * Terrain;
		CShader * Volume;
		CShader * Glyph;

		SShaders();
	};

	SShaders Shaders;

	CProgramContext();
	void run();

};

#endif
