#ifndef _CMAINSTATE_H_INCLUDED_
#define _CMAINSTATE_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "SciDataParser.h"
#include "SciDataTree.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/VerticalSlider.h>
#include <Gwen/Controls/ComboBox.h>


class CMainState : public CState<CMainState>, public Gwen::Event::Handler
{

	CCameraControl * Camera;
	ICameraSceneObject * OrbitCamera;
	CMeshSceneObject * Tyra;

	CMeshSceneObject * LightObject;

	ISceneObject * SoupObject;
	ISceneObject * VoxelObject;

	SVector3f LightPosition;
	CMesh * VolumeCube;

	CFrameBufferObject * VolumeTarget;
	CTexture * VolumeBuffer;

	CMeshSceneObject * SkyBox;

	Gwen::Controls::Canvas* pCanvas;

	int ShowVolume;
	bool ShowGUI;

	SciDataParser * DataParser[3];


	static int const ConsoleSize = 5;
	float ConsoleAccumulator;
	Gwen::Color ConsoleMessageColors[ConsoleSize];
	Gwen::Controls::Label * ConsoleMessages[ConsoleSize];

	Gwen::Controls::Label * VolumeRangeIndicator;

	float Slider;

	float Timer;

public:

	struct SVolumeControl
	{
		int Mode;
		SVector3f SliceAxis;
		float LocalRange;
		float MinimumAlpha;
		float EmphasisLocation;
		float AlphaIntensity;

		SVolumeControl()
			: Mode(0), SliceAxis(1.f, 0.f, 0.f),
			LocalRange(0.1f), MinimumAlpha(0.1f),
			EmphasisLocation(0.5f), AlphaIntensity(1.f)
		{}
	};

	SVolumeControl Volume;
	void resetVolumeRangeIndicator();
	void clearVolumeRangeIndicator();
	
	SUniformReference<SVector3f> BindLightPosition;

	CShader * Shader;
	CMesh * Cube;


	CMainState();

	void init();
	void initScene();

	void loadData();

	void begin();
	void OnRenderStart(float const Elapsed);

    void OnKeyboardEvent(SKeyboardEvent const & Event);

	void addConsoleMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255));


	void setupVolumeRender();

	//void OnEmphasisSlider(Gwen::Controls::Base* control);


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
