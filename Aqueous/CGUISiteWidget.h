
#pragma once

#include "CGUIContextWidget.h"


class CSite;

class CGUISiteWidget : public CGUIContextWidget
{

public:

	CGUISiteWidget(CSite * Site);
	
	void OnOpenSite(Gwen::Controls::Base * Control);

protected:

	void CreateSitePanels();

	Gwen::Controls::PageControl * PageControl;
	CSite * Site;

};
