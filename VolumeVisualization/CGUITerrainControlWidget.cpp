#include "CGUITerrainControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainSceneObject.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUITerrainControlWidget::CGUITerrainControlWidget()
	: Terrain(CProgramContext::get().Scene.Terrain), MainState(CMainState::get())
{
	Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 500, 330, 160);
	Window->SetTitle("Terrain Controls");

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText("Disable Terrain");
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
		ElevationButton->SetText("Elevation (Greyscale)");
		ElevationButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectElevation);

		Gwen::Controls::Button * ColorButton = new Gwen::Controls::Button(Window);
		ColorButton->SetBounds(140 + 15 + 10, 10 + 45 + 25, 140, 25);
		ColorButton->SetText("GoogleMaps (Color)");
		ColorButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectColor);

		// Wire Up Events
		//EmphasisSlider->onValueChanged.Add(		this,	& CGUIVolumeControlWidget::OnEmphasisSlider);
		//IntensitySlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnIntensitySlider);
		//MinimumAlphaSlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnMinimumAlphaSlider);
		//LocalRangeSlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnLocalRangeSlider);
	}
}

void CGUITerrainControlWidget::OnToggleTerrain(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();

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
	CProgramContext * Context = & CProgramContext::get();
	Context->Scene.Terrain->DebugHeight = true;
}

void CGUITerrainControlWidget::OnSelectColor(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::get();
	Context->Scene.Terrain->DebugHeight = false;
}

void CGUITerrainControlWidget::enable()
{
	Window->SetHidden(false);
}
