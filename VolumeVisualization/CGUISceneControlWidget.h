
#pragma once

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CGUISceneControlWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * EnableButton;

public:

	CGUISceneControlWidget();

	void OnToggleBackdrop(Gwen::Controls::Base * Control);
	void OnSelectPoint(Gwen::Controls::Base * Control);
	void OnSelectGrid(Gwen::Controls::Base * Control);

	void toggle();

};
