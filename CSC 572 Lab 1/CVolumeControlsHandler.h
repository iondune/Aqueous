#ifndef _CVOLUMECONTROLSHANDLER_H_INCLUDED_
#define _CVOLUMECONTROLSHANDLER_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "SciDataParser.h"
#include "SciDataTree.h"

#include "ColorMappers.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/VerticalSlider.h>
#include <Gwen/Controls/ComboBox.h>


class CVolumeControlsHandler : public Gwen::Event::Handler
{

public:
	SciDataParser * & Parser;
	float & Intensity;

	float LocalRange;
	float MinimumAlpha;
	float EmphasisLocation;

	bool SurfaceValues;

	static CVolumeControlsHandler * Instance;

	SVector3f AxisVector;

	CVolumeControlsHandler(SciDataParser * & pParser, float & intensity)
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
			Parser->createVolumeFromGridValues(& l);
		}
		else
		{
			COxygenLocalizedColorMapper l;
			l.EmphasisLocation = EmphasisLocation;
			l.MinimumAlpha = MinimumAlpha;
			l.LocalRange = LocalRange;
			l.SliceAxis = AxisVector;
			Parser->createVolumeFromGridValues(& l);
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
		Parser->createVolumeFromGridValues(& o);
	}

	void OnResetAlpha(Gwen::Controls::Base * Control)
	{
		Intensity = 1.f;
	}

	void OnSetXAxis(Gwen::Controls::Base * Control)
	{
		AxisVector = SVector3f(1.f, 0.f, 0.f);
		resetVolumeData();
	}

	void OnSetYAxis(Gwen::Controls::Base * Control)
	{
		AxisVector = SVector3f(0.f, 1.f, 0.f);
		resetVolumeData();
	}

	void OnSetZAxis(Gwen::Controls::Base * Control)
	{
		AxisVector = SVector3f(0.f, 0.f, 1.f);
		resetVolumeData();
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

#endif
