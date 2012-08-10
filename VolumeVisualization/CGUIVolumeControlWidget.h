#ifndef _CGUIVOLUMECONTROLWIDGET_H_INCLUDED_
#define _CGUIVOLUMECONTROLWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>

#include "CVolumeSceneObject.h"


class CMainState;

class CGUIVolumeControlWidget : public CGUIContextWidget
{

	CVolumeSceneObject::SControl & VolumeControl;
	CMainState & MainState;

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * EnableButton;

public:

	CGUIVolumeControlWidget();
	void resetVolumeRange();

	void OnEmphasisSlider(Gwen::Controls::Base * Control);
	void OnIntensitySlider(Gwen::Controls::Base * Control);
	void OnMinimumAlphaSlider(Gwen::Controls::Base * Control);
	void OnLocalRangeSlider(Gwen::Controls::Base * Control);

	void OnResetVolume(Gwen::Controls::Base * Control);
	void OnResetAlpha(Gwen::Controls::Base * Control);

	void OnSetXAxis(Gwen::Controls::Base * Control);
	void OnSetYAxis(Gwen::Controls::Base * Control);
	void OnSetZAxis(Gwen::Controls::Base * Control);

	void OnVolumeMode(Gwen::Controls::Base * Control);
	void OnToggleVolume(Gwen::Controls::Base * Control);

	void enable();

};

#endif