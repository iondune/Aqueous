#ifndef _CGUIMAINMENUSWIDGET_H_INCLUDED_
#define _CGUIMAINMENUSWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"


class SciDataManager;

class CGUIMainMenuWidget : public CGUIContextWidget
{

public:

	CGUIMainMenuWidget();

	void OnSelectDataSet(Gwen::Controls::Base * Control);

};

#endif
