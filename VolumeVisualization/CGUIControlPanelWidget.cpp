#include "CGUIControlPanelWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainSceneObject.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUIControlPanelWidget::CGUIControlPanelWidget()
{
	Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(30, 600, 660 + 30, 85);
	Window->SetTitle("Control Panel");
	Window->SetClosable(false);

	Gwen::Controls::Button * EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 150, 35);
	EnableButton->SetText("Volume Controls");
	EnableButton->onPress.Add(this, & CGUIControlPanelWidget::OnToggleVolume);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(180, 10, 150, 35);
	EnableButton->SetText("Terrain Controls");
	EnableButton->onPress.Add(this, & CGUIControlPanelWidget::OnToggleTerrain);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(345, 10, 150, 35);
	EnableButton->SetText("Glyph Controls");
	EnableButton->onPress.Add(this, & CGUIControlPanelWidget::OnToggleGlyph);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(510, 10, 150, 35);
	EnableButton->SetText("Scene Controls");
	EnableButton->onPress.Add(this, & CGUIControlPanelWidget::OnToggleScene);
}

void CGUIControlPanelWidget::OnToggleTerrain(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();
	Context->GUIContext->getTerrainControl()->toggle();
}

void CGUIControlPanelWidget::OnToggleVolume(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();
	Context->GUIContext->getVolumeControl()->toggle();
}

void CGUIControlPanelWidget::OnToggleGlyph(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();
	Context->GUIContext->getGlyphControl()->toggle();
}

void CGUIControlPanelWidget::OnToggleScene(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();
	Context->GUIContext->getSceneControl()->toggle();
}
