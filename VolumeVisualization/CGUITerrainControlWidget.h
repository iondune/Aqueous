
#pragma once

#include "CGUIContextWidget.h"
#include <ionScene.h>
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CMainState;
class CTerrainNodeManager;

class CGUITerrainControlWidget : public CGUIContextWidget
{

	CTerrainNodeManager * Terrain;
	CSceneNode * Water;
	CMainState & MainState;

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * TerrainButton, * WaterButton;

public:

	CGUITerrainControlWidget();
	
	void OnToggleTerrain(Gwen::Controls::Base * Control);
	void OnToggleWater(Gwen::Controls::Base * Control);
	void OnSelectElevation(Gwen::Controls::Base * Control);
	void OnSelectColor(Gwen::Controls::Base * Control);
	void OnModeSelect(Gwen::Controls::Base * Control);

	void toggle();

};
