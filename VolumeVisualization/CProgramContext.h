#ifndef _CPROGRAMCONTEXT_H_INCLUDED_
#define _CPROGRAMCONTEXT_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CGUIManager.h"

#include "SciDataParser.h"
#include "SciDataTree.h"

#include "CTerrainSceneObject.h"
#include "CVolumeSceneObject.h"


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

		// Default drawing shaders and mesh
		CShader * Shader;
		CMesh * Cube;


		SScene();
	};

	SScene Scene;
	CGUIManager * GUIManager;
	SciDataParser * DataParser[3];


	CProgramContext();

	void init();
	void run();

};

#endif
