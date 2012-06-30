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


class Handler1 : public Gwen::Event::Handler
{

public:
	SciDataParser * & Parser;
	float & Intensity;

	float LocalRange;
	float MinimumAlpha;
	float EmphasisLocation;

	bool SurfaceValues;

	static Handler1 * Instance;

	SVector3f AxisVector;

	Handler1(SciDataParser * & pParser, float & intensity)
		: Intensity(intensity), Parser(pParser)
	{
		LocalRange = 0.1f;
		MinimumAlpha = 0.03f;
		EmphasisLocation = 0.5f;
		SurfaceValues = false;
		AxisVector = SVector3f(1.f, 0.f, 0.f);

		if (! Instance)
			Instance = this;
	}

	void resetVolumeData()
	{
		if (SurfaceValues)
		{
			COxygenIsoSurfaceColorMapper l;
			l.EmphasisLocation = EmphasisLocation;
			l.MinimumAlpha = MinimumAlpha;
			l.LocalRange = LocalRange;
			Parser->generateVolumeFromGridValues(& l);
		}
		else
		{
			COxygenLocalizedColorMapper l;
			l.EmphasisLocation = EmphasisLocation;
			l.MinimumAlpha = MinimumAlpha;
			l.LocalRange = LocalRange;
			l.SliceAxis = AxisVector;
			Parser->generateVolumeFromGridValues(& l);
		}
	}

	void OnEmphasisSlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::VerticalSlider * Bar = (Gwen::Controls::VerticalSlider *) Control;
		//printf("Slider value: %f\n", Slider->GetValue());
		EmphasisLocation = Bar->GetValue();
		resetVolumeData();
	}

	void OnIntensitySlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::VerticalSlider * Bar = (Gwen::Controls::VerticalSlider *) Control;
		//printf("Slider value: %f\n", Slider->GetValue());
		Intensity = Bar->GetValue();
	}

	void OnMinimumAlphaSlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::VerticalSlider * Bar = (Gwen::Controls::VerticalSlider *) Control;
		MinimumAlpha = Bar->GetValue();
		resetVolumeData();
	}

	void OnLocalRangeSlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::VerticalSlider * Bar = (Gwen::Controls::VerticalSlider *) Control;
		LocalRange = Bar->GetValue();
		resetVolumeData();
	}

	void OnResetVolume(Gwen::Controls::Base * Control)
	{
		COxygenColorMapper o;
		Parser->generateVolumeFromGridValues(& o);
	}

	void OnResetAlpha(Gwen::Controls::Base * Control)
	{
		Intensity = 1.f;
	}

	void OnSetXAxis(Gwen::Controls::Base * Control)
	{
		AxisVector = SVector3f(1.f, 0.f, 0.f);
	}

	void OnSetYAxis(Gwen::Controls::Base * Control)
	{
		AxisVector = SVector3f(0.f, 1.f, 0.f);
	}

	void OnSetZAxis(Gwen::Controls::Base * Control)
	{
		AxisVector = SVector3f(0.f, 0.f, 1.f);
	}

	void OnVolumeMode(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

		if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Surface Values"))
			SurfaceValues = true;
		else
			SurfaceValues = false;
		resetVolumeData();
	}
};

class CMainState : public CState<CMainState>, public Gwen::Event::Handler
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

	Gwen::Controls::VerticalSlider * EmphasisSlider;



	float Slider;
	float AlphaIntensity;

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
