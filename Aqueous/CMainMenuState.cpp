
#include "CMainMenuState.h"

#include <ionGUI.h>

#include "CMainState.h"
#include "ColorMappers.h"
#include "CDataLoadingThread.h"
#include "CGlyphNodeManager.h"
#include "CGUIContext.h"

#include "CSite.h"


CMainMenuState::CMainMenuState()
{}

void CMainMenuState::Begin()
{
	Context->GUIContext->GetCanvas()->SetDrawBackground(true);
	CGUIMainMenuWidget * MainMenu = new CGUIMainMenuWidget();
	std::cout << "Menu State begin." << std::endl;
}

void CMainMenuState::End()
{
	Context->GUIContext->Clear();
	std::cout << "Menu State end." << std::endl;
}

void CMainMenuState::Update(f32 const Elapsed)
{
	std::chrono::milliseconds Milliseconds(1);
	std::this_thread::sleep_for(Milliseconds);

	Thread.Sync();

	Context->GUIContext->Manager->Draw(Elapsed, true);
}

void CMainMenuState::OnEvent(SWindowResizedEvent & Event)
{
	Context->GUIContext->GetCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->GetCanvas()->Invalidate();
	Context->GUIContext->GetCanvas()->InvalidateChildren(true);
}

void CMainMenuState::LoadData()
{
	Thread.Context = Context;
	Thread.LoadingWidget = new CGUIProgressBarWidget(Context->GUIContext->Manager.Get(), "Loading data and initializing scene elements");
	Thread.LoadingWidget->BeginProgress();
	Thread.Run();
}
