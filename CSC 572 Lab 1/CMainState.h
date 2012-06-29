#ifndef _CMAINSTATE_H_INCLUDED_
#define _CMAINSTATE_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "SciDataParser.h"
#include "SciDataTree.h"


#include <Gwen/Controls.h>

class CMainState : public CState<CMainState>
{

	CCameraControl * Camera;
	CMeshSceneObject * Tyra;

	CMeshSceneObject * LightObject;

	ISceneObject * SoupObject;
	ISceneObject * VoxelObject;

	SVector3f LightPosition;
	SUniformReference<SVector3f> BindLightPosition;

	CShader * Shader;
	CMesh * Cube;
	CMesh * VolumeCube;

	CFrameBufferObject * VolumeTarget;
	CTexture * VolumeBuffer;

	CMeshSceneObject * SkyBox;

	Gwen::Controls::Canvas* pCanvas;

	int ShowVolume;
	bool ShowGUI;

	SciDataParser * DataParser;


	static int const ConsoleSize = 5;
	float ConsoleAccumulator;
	Gwen::Color ConsoleMessageColors[ConsoleSize];
	Gwen::Controls::Label * ConsoleMessages[ConsoleSize];

public:

	CMainState();

	void init();
	void initScene();

	void loadData();

	void begin();
	void OnRenderStart(float const Elapsed);

    void OnKeyboardEvent(SKeyboardEvent const & Event);

	void addConsoleMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255));


	void setupVolumeRender();


	//////////////////////////////
	// Mouse Tracking Varaibles //
	//////////////////////////////

    glm::vec3 LastVec;
    int Mode;
	SVector3f Translation, Scale;
    glm::mat4 Rotation;
	

	////////////////////////////
	// Mouse Tracking Methods //
	////////////////////////////

    glm::vec3 makeSphereVec(int x, int y);
    void OnMouseEvent(SMouseEvent const & Event);

};

#endif
