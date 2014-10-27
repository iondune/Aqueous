
#pragma once

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CMainState;
class CTerrainNodeManager;
class CWaterSceneObject;

class CGUITerrainControlWidget : public CGUIContextWidget
{

	CTerrainNodeManager * Terrain;
	CWaterSceneObject * Water;
	CMainState & MainState;

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * TerrainButton, * WaterButton;

public:

	CGUITerrainControlWidget();
	
	void OnToggleTerrain(Gwen::Controls::Base * Control);
	void OnToggleWater(Gwen::Controls::Base * Control);
	void OnSelectElevation(Gwen::Controls::Base * Control);
	void OnSelectColor(Gwen::Controls::Base * Control);
	void OnSelectWireframe(Gwen::Controls::Base * Control);
	void OnSelectSolid(Gwen::Controls::Base * Control);

	void toggle();

};
