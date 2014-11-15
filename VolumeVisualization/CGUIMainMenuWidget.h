
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
	Gwen::Controls::PageControl * PageControl;
	Gwen::Controls::Base * SitePage;

	std::vector<Gwen::Controls::Button *> DataSetButtons;

	void CreateSitePanels();

public:

	CGUIMainMenuWidget();
	
	void OnSelectSite(Gwen::Controls::Base * Control);
	void OnSelectDataSet(Gwen::Controls::Base * Control);
	void OnCreateDataSet(Gwen::Controls::Base * Control);

};
