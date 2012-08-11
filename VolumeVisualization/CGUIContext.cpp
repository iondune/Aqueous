#include "CGUIContext.h"

#include "SciDataManager.h"
#include "CMainState.h"


CGUIContext::CGUIContext()
	: MainState(CMainState::get())
{}

void CGUIContext::setup()
{
	// Cleanup
	Canvas->RemoveAllChildren();
	Canvas->SetDrawBackground(false);

	// Widgets
	addWidget(Console = new CGUIConsoleWidget());
	addWidget(TitleLabels = new CGUITitleLabelsWidget());
	addWidget(VolumeControl = new CGUIVolumeControlWidget());
	addWidget(TerrainControl = new CGUITerrainControlWidget());
	addWidget(GlyphControl = new CGUIGlyphControlWidget());
	addWidget(ControlPanel = new CGUIControlPanelWidget());
	
	Console->addMessage("GUI Initialized.");
	Console->addMessage("Starting program...", Gwen::Colors::Red);
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

CGUIControlPanelWidget * CGUIContext::getControlPanel()
{
	return ControlPanel;
}
