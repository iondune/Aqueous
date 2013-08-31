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
	AddWidget(Console = new CGUIConsoleWidget());
	AddWidget(TitleLabels = new CGUITitleLabelsWidget(CProgramContext::Get().DataManager));
	AddWidget(VolumeControl = new CGUIVolumeControlWidget());
	AddWidget(TerrainControl = new CGUITerrainControlWidget());
	AddWidget(GlyphControl = new CGUIGlyphControlWidget());
	AddWidget(SceneControl = new CGUISceneControlWidget());
	AddWidget(ControlPanel = new CGUIControlPanelWidget());
	
	Console->addMessage("GUI Initialized.");
	Console->addMessage("Starting program...", Gwen::Colors::Red);
}

void CGUIContext::SetupMenuState()
{
	Canvas->SetDrawBackground(true);

	// Widgets
	AddWidget(MainMenu = new CGUIMainMenuWidget());
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
