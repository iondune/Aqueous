
#pragma once

#include <ionGUI.h>


class CGUIContext;

class CGUIContextWidget : public CGUIWidget
{

public:

	CGUIContextWidget();

protected:

	CGUIContext * GUIContext;

};
