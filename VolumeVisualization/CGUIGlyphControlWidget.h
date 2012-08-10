#ifndef _CGUIGLPYHCONTROLWIDGET_H_INCLUDED_
#define _CGUIGLPYHCONTROLWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CGUIGlyphControlWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * EnableButton;

public:

	CGUIGlyphControlWidget();

	void enable();

};

#endif
