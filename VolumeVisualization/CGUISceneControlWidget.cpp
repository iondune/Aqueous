#include "CGUISceneControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainNodeManager.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUISceneControlWidget::CGUISceneControlWidget()
{
	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 670, 330, 160);
	Window->SetTitle("Scene Controls");
	Window->SetHidden(true);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText("Enable Backdrop");
	EnableButton->onPress.Add(this, & CGUISceneControlWidget::OnToggleBackdrop);
}

void CGUISceneControlWidget::OnToggleBackdrop(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	
	Context->Scene.SkyBox->SetVisible(! Context->Scene.SkyBox->IsVisible());
	std::stringstream s;
	s << "Skybox object ";
	if (Context->Scene.SkyBox->IsVisible())
		s << "enabled.";
	else
		s << "disabled.";
	Context->GUIContext->GetConsole()->AddMessage(s.str());

	if (Context->Scene.SkyBox->IsVisible())
		((Gwen::Controls::Button *) Control)->SetText("Disable Backdrop");
	else
		((Gwen::Controls::Button *) Control)->SetText("Enable Backdrop");
}

void CGUISceneControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
