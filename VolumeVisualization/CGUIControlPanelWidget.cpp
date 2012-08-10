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
	Window->SetBounds(30, 600, 495 + 30, 85);
	Window->SetTitle("Control Panel");
	Window->SetClosable(false);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 150, 35);
	EnableButton->SetText("Volume Controls");

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(180, 10, 150, 35);
	EnableButton->SetText("Terrain Controls");

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(345, 10, 150, 35);
	EnableButton->SetText("Glyph Controls");
	//EnableButton->onPress.Add(this, & CGUIVolumeControlWidget::OnToggleVolume);

	// Slider Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->getRegularFont());
		SliderLabel->SetText(L"Mode:");
		SliderLabel->SetBounds(10, 10 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::Button * pButtonX = new Gwen::Controls::Button(Window);
		pButtonX->SetBounds(15, 10 + 45 + 25, 140, 25);
		pButtonX->SetText("Source Points");

		Gwen::Controls::Button * pButtonY = new Gwen::Controls::Button(Window);
		pButtonY->SetBounds(140 + 15 + 10, 10 + 45 + 25, 140, 25);
		pButtonY->SetText("Grid Samples");

		// Wire Up Events
		//EmphasisSlider->onValueChanged.Add(		this,	& CGUIVolumeControlWidget::OnEmphasisSlider);
		//IntensitySlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnIntensitySlider);
		//MinimumAlphaSlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnMinimumAlphaSlider);
		//LocalRangeSlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnLocalRangeSlider);
	}
}

void CGUIControlPanelWidget::enable()
{
	Window->SetHidden(false);
}
