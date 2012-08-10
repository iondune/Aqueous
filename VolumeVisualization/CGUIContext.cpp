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
	new CGUITerrainControlWidget();
	new CGUIGlyphControlWidget();
	new CGUIControlPanelWidget();
	
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
