#include "CGUITerrainControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainNodeManager.h"
#include "CWaterSceneObject.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUITerrainControlWidget::CGUITerrainControlWidget()
	: Terrain(CProgramContext::Get().Scene.Terrain), Water(CProgramContext::Get().Scene.Water), MainState(CMainState::Get())
{
	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 500, 330, 235);
	Window->SetTitle("Terrain Controls");
	Window->SetHidden(true);

	TerrainButton = new Gwen::Controls::Button(Window);
	TerrainButton->SetBounds(15, 10, 290, 25);
	TerrainButton->SetText(Terrain->GetNode()->IsVisible() ? "Disable Terrain" : "Enable Terrain");
	TerrainButton->onPress.Add(this, & CGUITerrainControlWidget::OnToggleTerrain);

	WaterButton = new Gwen::Controls::Button(Window);
	WaterButton->SetBounds(15, 10 + 35, 290, 25);
	//WaterButton->SetText(Water->isVisible() ? "Disable Water" : "Enable Water");
	WaterButton->onPress.Add(this, & CGUITerrainControlWidget::OnToggleWater);

	// Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Texture:");
		SliderLabel->SetBounds(10, 10 + 30 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::Button * ElevationButton = new Gwen::Controls::Button(Window);
		ElevationButton->SetBounds(15, 10 + 30 + 45 + 20, 140, 25);
		ElevationButton->SetText("Elevation");
		ElevationButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectElevation);

		Gwen::Controls::Button * ColorButton = new Gwen::Controls::Button(Window);
		ColorButton->SetBounds(140 + 15 + 10, 10 + 30 + 45 + 20, 140, 25);
		ColorButton->SetText("GoogleMaps");
		ColorButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectColor);


		Gwen::Controls::Label * ModeLabel = new Gwen::Controls::Label(Window);
		ModeLabel->SetFont(GUIManager->GetRegularFont());
		ModeLabel->SetText(L"Mode:");
		ModeLabel->SetBounds(10, 10 + 15 + 45 + 75, 300, 40);
		ModeLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::Button * SolidButton = new Gwen::Controls::Button(Window);
		SolidButton->SetBounds(15, 10 + 15 + 45 + 20 + 75, 140, 25);
		SolidButton->SetText("Solid");
		SolidButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectSolid);

		Gwen::Controls::Button * WireframeButton = new Gwen::Controls::Button(Window);
		WireframeButton->SetBounds(140 + 15 + 10, 10 + 15 + 45 + 20 + 75, 140, 25);
		WireframeButton->SetText("Wireframe");
		WireframeButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectWireframe);
	}
}

void CGUITerrainControlWidget::OnToggleTerrain(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	if (Context->Scene.Terrain->GetNode()->IsVisible())
	{
		Context->Scene.Terrain->GetNode()->SetVisible(false);
		GUIContext->GetConsole()->AddMessage("Terrain View Disabled");
		TerrainButton->SetText("Enable Terrain");
	}
	else
	{
		Context->Scene.Terrain->GetNode()->SetVisible(true);
		GUIContext->GetConsole()->AddMessage("Terrain View Enabled");
		TerrainButton->SetText("Disable Terrain");
	}
}

void CGUITerrainControlWidget::OnToggleWater(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	//if (Context->Scene.Water->isVisible())
	//{
	//	Context->Scene.Water->setVisible(false);
	//	GUIContext->GetConsole()->AddMessage("Water View Disabled");
	//	WaterButton->SetText("Enable Water");
	//}
	//else
	//{
	//	Context->Scene.Water->setVisible(true);
	//	GUIContext->GetConsole()->AddMessage("Water View Enabled");
	//	WaterButton->SetText("Disable Water");
	//}
}

void CGUITerrainControlWidget::OnSelectElevation(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->SetDebugHeightEnabled(true);
}

void CGUITerrainControlWidget::OnSelectColor(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->SetDebugHeightEnabled(false);
}

void CGUITerrainControlWidget::OnSelectSolid(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
}

void CGUITerrainControlWidget::OnSelectWireframe(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, true);
}

void CGUITerrainControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
