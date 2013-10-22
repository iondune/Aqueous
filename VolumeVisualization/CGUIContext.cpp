#include "CGUIContext.h"

#include "SciDataManager.h"
#include "CMainState.h"


CGUIContext::CGUIContext()
	: MainState(CMainState::Get()), Console(0), TitleLabels(0), VolumeControl(0),
	TerrainControl(0), GlyphControl(0), SceneControl(0), ControlPanel(0), MainMenu(0)
{}

void CGUIContext::SetupMainState()
{
	Canvas->SetDrawBackground(false);

	// Widgets
	Console = new CGUIConsoleWidget(this);
	TitleLabels = new CGUITitleLabelsWidget(CProgramContext::Get().CurrentSite);
	VolumeControl = new CGUIVolumeControlWidget();
	TerrainControl = new CGUITerrainControlWidget();
	GlyphControl = new CGUIGlyphControlWidget();
	SceneControl = new CGUISceneControlWidget();
	ControlPanel = new CGUIControlPanelWidget();
	
	//Console->AddMessage("GUI Initialized.");
	//Console->AddMessage("Starting program...", Colors::Red);
}

void CGUIContext::SetupMenuState()
{
	Canvas->SetDrawBackground(true);

	// Widgets
	MainMenu = new CGUIMainMenuWidget();
}

void CGUIContext::Clear()
{
	Console = 0;
	TitleLabels = 0;
	VolumeControl = 0;
	TerrainControl = 0;
	GlyphControl = 0;
	SceneControl = 0;
	ControlPanel = 0;

	MainMenu = 0;

	RemoveAllWidgets();
	Canvas->RemoveAllChildren();
}

CGUIConsoleWidget * CGUIContext::GetConsole()
{
	return Console;
}

CGUITitleLabelsWidget * CGUIContext::GetTitleLabels()
{
	return TitleLabels;
}

CGUIVolumeControlWidget * CGUIContext::GetVolumeControl()
{
	return VolumeControl;
}

CGUITerrainControlWidget * CGUIContext::GetTerrainControl()
{
	return TerrainControl;
}

CGUIGlyphControlWidget * CGUIContext::GetGlyphControl()
{
	return GlyphControl;
}

CGUISceneControlWidget * CGUIContext::GetSceneControl()
{
	return SceneControl;
}

CGUIControlPanelWidget * CGUIContext::GetControlPanel()
{
	return ControlPanel;
}
