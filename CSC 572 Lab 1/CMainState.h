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


#include "CTerrainSceneObject.h"
#include "CConsole.h"


class CMainState : public CState<CMainState>, public Gwen::Event::Handler
{

	// Cameras
	CCameraControl * Camera;
	ICameraSceneObject * OrbitCamera;

	// Lighting
	CMeshSceneObject * LightObject;
	SVector3f LightPosition;

	// Holders for point cloud and grid glyph representations
	ISceneObject * PointCloudObject;
	ISceneObject * GridObject;

	// Cube used for volume rendering
	CMesh * VolumeCube;

	// Framebuffer used for volume rendering
	CFrameBufferObject * VolumeTarget;
	CTexture * VolumeBuffer;

	// Skybox object
	CMeshSceneObject * SkyBox;

	// GUI Canvas
	Gwen::Controls::Canvas* pCanvas;

	// Settings
	int ShowVolume;
	bool ShowGUI;

	SciDataParser * DataParser[3];

	CConsole * Console;


	

	Gwen::Controls::Label * VolumeRangeIndicator;

	float Slider;

	float Timer;



	CTerrainSceneObject * Terrain;

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

	void addConsoleMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255))
	{
		Console->addMessage(Message, Color);
	}


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
