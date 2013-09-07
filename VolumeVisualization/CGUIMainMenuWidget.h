
#pragma once

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/MenuStrip.h>
#include <Gwen/Controls/ListBox.h>


class SciDataManager;

class CGUIMainMenuWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::MenuStrip * Menu;
	Gwen::Controls::ListBox * ListBox;

	Gwen::Controls::Button * NewDataSetButton;

	std::vector<Gwen::Controls::Button *> DataSetButtons;

	void createDataSetButtons();

public:

	CGUIMainMenuWidget();

	void OnSelectDataSet(Gwen::Controls::Base * Control);
	void OnCreateDataSet(Gwen::Controls::Base * Control);

};
