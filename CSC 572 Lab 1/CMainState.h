#ifndef _CMAINSTATE_H_INCLUDED_
#define _CMAINSTATE_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "SciDataParser.h"

class CMainState : public CState<CMainState>
{

	CCameraControl * Camera;
	CMeshSceneObject * Tyra;

	CMeshSceneObject * LightObject;

	ISceneObject * SoupObject;
	ISceneObject * VoxelObject;

	SVector3f LightPosition;
	SUniformReference<SVector3f> BindLightPosition;

	ISciTreeNode * DataTree;

	CShader * Shader;
	CMesh * Cube;

public:

	SciDataParser DataSet;

	CMainState();

	void init();
	void initScene();

	void loadData();

	void begin();
	void OnRenderStart(float const Elapsed);

	void parseMatFiles();

	
    void OnKeyboardEvent(SKeyboardEvent const & Event);

    glm::vec3 LastVec;

    glm::vec3 makeSphereVec(int x, int y);

    int Mode;
	SVector3f Translation, Scale;
    glm::mat4 Rotation;

    void OnMouseEvent(SMouseEvent const & Event);

};

#endif
