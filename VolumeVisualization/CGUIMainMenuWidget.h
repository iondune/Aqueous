#ifndef _CGUIMAINMENUSWIDGET_H_INCLUDED_
#define _CGUIMAINMENUSWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"


class SciDataManager;

class CGUIMainMenuWidget : public CGUIContextWidget
{

	Gwen::Controls::Button * NewDataSetButton;

	std::vector<Gwen::Controls::Button *> DataSetButtons;

	void createDataSetButtons();

public:

	CGUIMainMenuWidget();

	void OnSelectDataSet(Gwen::Controls::Base * Control);

};

#endif
