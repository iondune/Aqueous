#ifndef _CGUICONTEXT_H_INCLUDED_
#define _CGUICONTEXT_H_INCLUDED_

#include "CGUIManager.h"

#include "CGUIConsoleWidget.h"
#include "CGUITitleLabelsWidget.h"
#include "CGUIVolumeControlWidget.h"
#include "CGUITerrainControlWidget.h"
#include "CGUIGlyphControlWidget.h"
#include "CGUISceneControlWidget.h"
#include "CGUIControlPanelWidget.h"


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

public:

	CGUIContext();

	void setup();
	
	CGUIConsoleWidget * getConsole();
	CGUITitleLabelsWidget * getTitleLabels();
	CGUIVolumeControlWidget * getVolumeControl();
	CGUITerrainControlWidget * getTerrainControl();
	CGUIGlyphControlWidget * getGlyphControl();
	CGUISceneControlWidget * getSceneControl();
	CGUIControlPanelWidget * getControlPanel();

};

#endif
