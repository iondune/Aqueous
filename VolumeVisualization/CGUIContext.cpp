#include "CGUIContext.h"

#include "SciDataManager.h"
#include "CMainState.h"


CGUIContext::CGUIContext()
	: MainState(CMainState::get()), Console(0), TitleLabels(0), VolumeControl(0),
	TerrainControl(0), GlyphControl(0), SceneControl(0), ControlPanel(0), MainMenu(0)
{}

void CGUIContext::setupMainState()
{
	//Canvas->SetDrawBackground(false);

	// Widgets
	addWidget(Console = new CGUIConsoleWidget());
	addWidget(TitleLabels = new CGUITitleLabelsWidget(CProgramContext::get().DataManager));
	addWidget(VolumeControl = new CGUIVolumeControlWidget());
	addWidget(TerrainControl = new CGUITerrainControlWidget());
	addWidget(GlyphControl = new CGUIGlyphControlWidget());
	addWidget(SceneControl = new CGUISceneControlWidget());
	addWidget(ControlPanel = new CGUIControlPanelWidget());
	
	//Console->addMessage("GUI Initialized.");
	//Console->addMessage("Starting program...", Gwen::Colors::Red);
}

void CGUIContext::setupMenuState()
{
	//Canvas->SetDrawBackground(true);

	// Widgets
	addWidget(MainMenu = new CGUIMainMenuWidget());
}

void CGUIContext::clear()
{
	Console = 0;
	TitleLabels = 0;
	VolumeControl = 0;
	TerrainControl = 0;
	GlyphControl = 0;
	SceneControl = 0;
	ControlPanel = 0;

	MainMenu = 0;

	removeAllWidgets();
	//Canvas->RemoveAllChildren();
}

CGUIConsoleWidget * CGUIContext::getConsole()
{
	return Console;
}

CGUITitleLabelsWidget * CGUIContext::getTitleLabels()
{
	return TitleLabels;
}

CGUIVolumeControlWidget * CGUIContext::getVolumeControl()
{
	return VolumeControl;
}

CGUITerrainControlWidget * CGUIContext::getTerrainControl()
{
	return TerrainControl;
}

CGUIGlyphControlWidget * CGUIContext::getGlyphControl()
{
	return GlyphControl;
}

CGUISceneControlWidget * CGUIContext::getSceneControl()
{
	return SceneControl;
}

CGUIControlPanelWidget * CGUIContext::getControlPanel()
{
	return ControlPanel;
}
