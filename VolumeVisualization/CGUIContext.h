
#pragma once

#include <ionGUI.h>

// Main State
#include "CGUITitleLabelsWidget.h"
#include "CGUIVolumeControlWidget.h"
#include "CGUITerrainControlWidget.h"
#include "CGUIGlyphControlWidget.h"
#include "CGUISceneControlWidget.h"
#include "CGUIControlPanelWidget.h"

// Menu State
#include "CGUIMainMenuWidget.h"


class CMainState;

class CGUIContext : public Singleton<CGUIContext>
{

	friend class Singleton<CGUIContext>;

public:

	void SetupMainState();
	void Clear();
	void Init();

	CGUIConsoleWidget * GetConsole();
	CGUITitleLabelsWidget * GetTitleLabels();
	CGUIVolumeControlWidget * GetVolumeControl();
	CGUITerrainControlWidget * GetTerrainControl();
	CGUIGlyphControlWidget * GetGlyphControl();
	CGUISceneControlWidget * GetSceneControl();
	CGUIControlPanelWidget * GetControlPanel();

	Gwen::Controls::Canvas * GetCanvas();

	SingletonPointer<CGUIManager> Manager;

protected:

	SingletonPointer<CMainState> MainState;

	CGUIConsoleWidget * Console = 0;
	CGUITitleLabelsWidget * TitleLabels = 0;
	CGUIVolumeControlWidget * VolumeControl = 0;
	CGUITerrainControlWidget * TerrainControl = 0;
	CGUIGlyphControlWidget * GlyphControl = 0;
	CGUISceneControlWidget * SceneControl = 0;
	CGUIControlPanelWidget * ControlPanel = 0;

private:

	CGUIContext()
	{}

};
