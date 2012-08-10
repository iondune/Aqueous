#ifndef _CGUICONTEXTWIDGET_H_INCLUDED_
#define _CGUICONTEXTWIDGET_H_INCLUDED_

#include "CGUIManager.h"
#include "CGUIWidget.h"


class CGUIContext;

class CGUIContextWidget : public CGUIWidget
{

protected:

	CGUIContext * GUIContext;

public:

	CGUIContextWidget();

};

#endif