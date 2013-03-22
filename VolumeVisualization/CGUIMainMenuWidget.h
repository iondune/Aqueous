#ifndef _CGUIMAINMENUSWIDGET_H_INCLUDED_
#define _CGUIMAINMENUSWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/MenuStrip.h>


class SciDataManager;

class CGUIMainMenuWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::MenuStrip * Menu;

	Gwen::Controls::Button * NewDataSetButton;

	std::vector<Gwen::Controls::Button *> DataSetButtons;

	void createDataSetButtons();

public:

	CGUIMainMenuWidget();

	void OnSelectDataSet(Gwen::Controls::Base * Control);

};

#endif
