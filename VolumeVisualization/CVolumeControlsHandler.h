#ifndef _CVOLUMECONTROLSHANDLER_H_INCLUDED_
#define _CVOLUMECONTROLSHANDLER_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include <Gwen/Controls.h>
#include <Gwen/Controls/VerticalSlider.h>
#include <Gwen/Controls/ComboBox.h>

#include "SciDataParser.h"
#include "SciDataTree.h"

#include "ColorMappers.h"

#include "CVolumeSceneObject.h"
#include "CMainState.h"
#include "CProgramContext.h"


class CVolumeControlsHandler : public Gwen::Event::Handler
{

public:

	CVolumeSceneObject::SControl & VolumeControl;
	CMainState & MainState;

	CVolumeControlsHandler()
		: VolumeControl(CProgramContext::get().Scene.VolumeSceneObject->Control), MainState(CMainState::get())
	{}

	void resetVolumeRange()
	{
		if (VolumeControl.Mode)
			CProgramContext::get().GUIManager->resetVolumeRangeIndicator(CProgramContext::get().DataParser[2]);
	}

	void OnEmphasisSlider(Gwen::Controls::Base * Control)
	{
		Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
		VolumeControl.EmphasisLocation = Bar->GetValue();
		resetVolumeRange();
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
		resetVolumeRange();
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
			resetVolumeRange();
		}
		else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Isosurface"))
		{
			VolumeControl.Mode = 2;
			resetVolumeRange();
		}
		else
		{
			VolumeControl.Mode = 0;
			CProgramContext::get().GUIManager->clearVolumeRangeIndicator();
		}
	}
};

#endif
