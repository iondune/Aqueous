#ifndef _CGUIVOLUMECONTROLWIDGET_H_INCLUDED_
#define _CGUIVOLUMECONTROLWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"

#include "CVolumeSceneObject.h"


class CMainState;

class CGUIVolumeControlWidget : public CGUIContextWidget
{

public:

	CVolumeSceneObject::SControl & VolumeControl;
	CMainState & MainState;

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

};

#endif
