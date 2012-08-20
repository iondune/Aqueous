#include "CMainMenuState.h"


CMainMenuState::CMainMenuState()
{}

void CMainMenuState::begin()
{
	Context->GUIContext->setupMenuState();
}

void CMainMenuState::end()
{
	Context->GUIContext->clear();
}

void CMainMenuState::OnRenderStart(float const Elapsed)
{
	Context->GUIContext->draw(Elapsed, true);
	CApplication::get().swapBuffers();
}

void CMainMenuState::OnWindowResized(SWindowResizedEvent const & Event)
{
	Context->GUIContext->getCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->getCanvas()->Invalidate();
	Context->GUIContext->getCanvas()->InvalidateChildren(true);
}
