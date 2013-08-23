
#pragma once

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
