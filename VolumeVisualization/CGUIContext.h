
#pragma once

#include "CGUIManager.h"

// Main State
#include "CGUIConsoleWidget.h"
#include "CGUITitleLabelsWidget.h"
#include "CGUIVolumeControlWidget.h"
#include "CGUITerrainControlWidget.h"
#include "CGUIGlyphControlWidget.h"
#include "CGUISceneControlWidget.h"
#include "CGUIControlPanelWidget.h"

// Menu State
#include "CGUIMainMenuWidget.h"


class CMainState;

class CGUIContext : public CGUIManager
{

protected:

	CMainState & MainState;
	
	CGUIConsoleWidget * Console;
	CGUITitleLabelsWidget * TitleLabels;
	CGUIVolumeControlWidget * VolumeControl;
	CGUITerrainControlWidget * TerrainControl;
	CGUIGlyphControlWidget * GlyphControl;
	CGUISceneControlWidget * SceneControl;
	CGUIControlPanelWidget * ControlPanel;
	CGUIMainMenuWidget * MainMenu;

public:

	CGUIContext();

	void setupMainState();
	void setupMenuState();
	void clear();
	
	CGUIConsoleWidget * getConsole();
	CGUITitleLabelsWidget * getTitleLabels();
	CGUIVolumeControlWidget * getVolumeControl();
	CGUITerrainControlWidget * getTerrainControl();
	CGUIGlyphControlWidget * getGlyphControl();
	CGUISceneControlWidget * getSceneControl();
	CGUIControlPanelWidget * getControlPanel();

};
