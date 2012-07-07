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

	//SciDataParser ** & Parser;
	CMainState::SVolumeControl & VolumeControl;

	CVolumeControlsHandler(/*SciDataParser ** & pParser*/)
		: /*Parser(pParser), */VolumeControl(CMainState::get().Volume)
	{}

	void OnEmphasisSlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
		VolumeControl.EmphasisLocation = Bar->GetValue();
		if (VolumeControl.Mode)
			CMainState::get().resetVolumeRangeIndicator();
	}

	void OnIntensitySlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
		VolumeControl.AlphaIntensity = Bar->GetValue();
	}

	void OnMinimumAlphaSlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
		VolumeControl.MinimumAlpha = Bar->GetValue();
	}

	void OnLocalRangeSlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
		VolumeControl.LocalRange = Bar->GetValue();
		if (VolumeControl.Mode)
			CMainState::get().resetVolumeRangeIndicator();
	}

	void OnResetVolume(Gwen::Controls::Base * Control)
	{
		VolumeControl.Mode = 0;
	}

	void OnResetAlpha(Gwen::Controls::Base * Control)
	{
		VolumeControl.AlphaIntensity = 1.f;
	}

	void OnSetXAxis(Gwen::Controls::Base * Control)
	{
		VolumeControl.SliceAxis = SVector3f(1.f, 0.f, 0.f);
	}

	void OnSetYAxis(Gwen::Controls::Base * Control)
	{
		VolumeControl.SliceAxis = SVector3f(0.f, 1.f, 0.f);
	}

	void OnSetZAxis(Gwen::Controls::Base * Control)
	{
		VolumeControl.SliceAxis = SVector3f(0.f, 0.f, 1.f);
	}

	void OnVolumeMode(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

		if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Plane Slices"))
		{
			VolumeControl.Mode = 1;
			CMainState::get().resetVolumeRangeIndicator();
		}
		else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Isosurface"))
		{
			VolumeControl.Mode = 2;
			CMainState::get().resetVolumeRangeIndicator();
		}
		else
		{
			VolumeControl.Mode = 0;
			CMainState::get().clearVolumeRangeIndicator();
		}
	}
};

#endif
