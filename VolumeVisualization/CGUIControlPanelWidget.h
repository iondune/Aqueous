
#pragma once

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CGUIControlPanelWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;

public:

	CGUIControlPanelWidget();

	void OnToggleTerrain(Gwen::Controls::Base * Control);
	void OnToggleVolume(Gwen::Controls::Base * Control);
	void OnToggleGlyph(Gwen::Controls::Base * Control);
	void OnToggleScene(Gwen::Controls::Base * Control);

};
