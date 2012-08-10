#ifndef _CGUITERRAINCONTROLWIDGET_H_INCLUDED_
#define _CGUITERRAINCONTROLWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CMainState;
class CTerrainSceneObject;

class CGUITerrainControlWidget : public CGUIContextWidget
{

	CTerrainSceneObject * Terrain;
	CMainState & MainState;

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * EnableButton;

public:

	CGUITerrainControlWidget();

	void enable();

};

#endif
