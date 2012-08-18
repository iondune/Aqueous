#include "CGUISceneControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainSceneObject.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUISceneControlWidget::CGUISceneControlWidget()
{
	Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
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
	CProgramContext * Context = & CProgramContext::get();
	
	Context->Scene.SkyBox->setVisible(! Context->Scene.SkyBox->isVisible());
	std::stringstream s;
	s << "Skybox object ";
	if (Context->Scene.SkyBox->isVisible())
		s << "enabled.";
	else
		s << "disabled.";
	Context->GUIContext->getConsole()->addMessage(s.str());

	if (Context->Scene.SkyBox->isVisible())
		((Gwen::Controls::Button *) Control)->SetText("Disable Backdrop");
	else
		((Gwen::Controls::Button *) Control)->SetText("Enable Backdrop");
}

void CGUISceneControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
