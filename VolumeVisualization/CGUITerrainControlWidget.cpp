#include "CGUITerrainControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainSceneObject.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUITerrainControlWidget::CGUITerrainControlWidget()
	: Terrain(CProgramContext::Get().Scene.Terrain), MainState(CMainState::Get())
{
	Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 500, 330, 235);
	Window->SetTitle("Terrain Controls");
	Window->SetHidden(true);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText(Terrain->isVisible() ? "Disable Terrain" : "Enable Terrain");
	EnableButton->onPress.Add(this, & CGUITerrainControlWidget::OnToggleTerrain);

	// Slider Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->getRegularFont());
		SliderLabel->SetText(L"Texture:");
		SliderLabel->SetBounds(10, 10 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::Button * ElevationButton = new Gwen::Controls::Button(Window);
		ElevationButton->SetBounds(15, 10 + 45 + 25, 140, 25);
		ElevationButton->SetText("Elevation");
		ElevationButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectElevation);

		Gwen::Controls::Button * ColorButton = new Gwen::Controls::Button(Window);
		ColorButton->SetBounds(140 + 15 + 10, 10 + 45 + 25, 140, 25);
		ColorButton->SetText("GoogleMaps");
		ColorButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectColor);


		Gwen::Controls::Label * ModeLabel = new Gwen::Controls::Label(Window);
		ModeLabel->SetFont(GUIManager->getRegularFont());
		ModeLabel->SetText(L"Mode:");
		ModeLabel->SetBounds(10, 10 + 45 + 75, 300, 40);
		ModeLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::Button * SolidButton = new Gwen::Controls::Button(Window);
		SolidButton->SetBounds(15, 10 + 45 + 25 + 75, 140, 25);
		SolidButton->SetText("Solid");
		SolidButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectSolid);

		Gwen::Controls::Button * WireframeButton = new Gwen::Controls::Button(Window);
		WireframeButton->SetBounds(140 + 15 + 10, 10 + 45 + 25 + 75, 140, 25);
		WireframeButton->SetText("Wireframe");
		WireframeButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectWireframe);
	}
}

void CGUITerrainControlWidget::OnToggleTerrain(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	if (Context->Scene.Terrain->isVisible())
	{
		Context->Scene.Terrain->setVisible(false);
		GUIContext->getConsole()->addMessage("Terrain View Disabled");
		EnableButton->SetText("Enable Terrain");
	}
	else
	{
		Context->Scene.Terrain->setVisible(true);
		GUIContext->getConsole()->addMessage("Terrain View Enabled");
		EnableButton->SetText("Disable Terrain");
	}
}

void CGUITerrainControlWidget::OnSelectElevation(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->DebugHeight = true;
}

void CGUITerrainControlWidget::OnSelectColor(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->DebugHeight = false;
}

void CGUITerrainControlWidget::OnSelectSolid(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->disableDebugData(EDebugData::Wireframe);
}

void CGUITerrainControlWidget::OnSelectWireframe(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->enableDebugData(EDebugData::Wireframe);
}

void CGUITerrainControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
