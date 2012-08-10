#ifndef _CGUICONTROLPANELWIDGET_H_INCLUDED_
#define _CGUICONTROLPANELWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CGUIControlPanelWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * EnableButton;

public:

	CGUIControlPanelWidget();

	void enable();

};

#endif
