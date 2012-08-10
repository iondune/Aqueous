#include "CGUIVolumeControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/HorizontalSlider.h>
#include <Gwen/Controls/ComboBox.h>


CGUIVolumeControlWidget::CGUIVolumeControlWidget()
	: VolumeControl(CProgramContext::get().Scene.VolumeSceneObject->Control), MainState(CMainState::get())
{
	// Slider Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
		SliderLabel->SetFont(GUIManager->getRegularFont());
		SliderLabel->SetText(L"Selection Value:");
		SliderLabel->SetBounds(1250, 10, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

		Gwen::Controls::HorizontalSlider * EmphasisSlider = new Gwen::Controls::HorizontalSlider(GUIManager->getCanvas());
		EmphasisSlider->SetBounds(1250, 30, 300, 40);
		EmphasisSlider->SetRange(0.f, 1.f);

		SliderLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
		SliderLabel->SetFont(GUIManager->getRegularFont());
		SliderLabel->SetText(L"Volume Opacity:");
		SliderLabel->SetBounds(1250, 70, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

		Gwen::Controls::HorizontalSlider * IntensitySlider = new Gwen::Controls::HorizontalSlider(GUIManager->getCanvas());
		IntensitySlider->SetBounds(1250, 90, 300, 40);
		IntensitySlider->SetRange(10.f, 0.5f);

		SliderLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
		SliderLabel->SetFont(GUIManager->getRegularFont());
		SliderLabel->SetText(L"Selection Range:");
		SliderLabel->SetBounds(1250, 130, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

		Gwen::Controls::HorizontalSlider * LocalRangeSlider = new Gwen::Controls::HorizontalSlider(GUIManager->getCanvas());
		LocalRangeSlider->SetBounds(1250, 150, 300, 40);
		LocalRangeSlider->SetRange(0.05f, 0.5f);

		SliderLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
		SliderLabel->SetFont(GUIManager->getRegularFont());
		SliderLabel->SetText(L"Selection Difference:");
		SliderLabel->SetBounds(1250, 190, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

		Gwen::Controls::HorizontalSlider * MinimumAlphaSlider = new Gwen::Controls::HorizontalSlider(GUIManager->getCanvas());
		MinimumAlphaSlider->SetBounds(1250, 210, 300, 40);
		MinimumAlphaSlider->SetRange(0.0f, 0.5f);

		// Wire Up Events
		EmphasisSlider->onValueChanged.Add(		this,	& CGUIVolumeControlWidget::OnEmphasisSlider);
		IntensitySlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnIntensitySlider);
		MinimumAlphaSlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnMinimumAlphaSlider);
		LocalRangeSlider->onValueChanged.Add(	this,	& CGUIVolumeControlWidget::OnLocalRangeSlider);
	}

	
	// Other Controls Panel
	{
		Gwen::Controls::Label * ControlLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
		ControlLabel->SetFont(GUIManager->getRegularFont());
		ControlLabel->SetText(L"Mode:");
		ControlLabel->SetBounds(1300, 320, 90, 40);
		ControlLabel->SetTextColor(Gwen::Color(255, 255, 200, 215));

		Gwen::Controls::ComboBox * VolumeMode = new Gwen::Controls::ComboBox(GUIManager->getCanvas());
		VolumeMode->SetBounds(1350, 120 + 120 + 45 + 35, 200, 25);
		VolumeMode->AddItem(L"Full Volume");
		VolumeMode->AddItem(L"Plane Slices");
		VolumeMode->AddItem(L"Isosurface");

		Gwen::Controls::Button * pButton2 = new Gwen::Controls::Button(GUIManager->getCanvas());
		pButton2->SetBounds(1350, 120 + 120 + 10 + 35, 200, 25);
		pButton2->SetText("Reset Alpha Intensity");
	
		ControlLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
		ControlLabel->SetFont(GUIManager->getRegularFont());
		ControlLabel->SetText(L"Axis:");
		ControlLabel->SetBounds(1313, 355, 90, 40);
		ControlLabel->SetTextColor(Gwen::Color(255, 255, 200, 215));

		Gwen::Controls::Button * pButtonX = new Gwen::Controls::Button(GUIManager->getCanvas());
		pButtonX->SetBounds(1350, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
		pButtonX->SetText("X");

		Gwen::Controls::Button * pButtonY = new Gwen::Controls::Button(GUIManager->getCanvas());
		pButtonY->SetBounds(1400, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
		pButtonY->SetText("Y");

		Gwen::Controls::Button * pButtonZ = new Gwen::Controls::Button(GUIManager->getCanvas());
		pButtonZ->SetBounds(1450, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
		pButtonZ->SetText("Z");

		// Wire Up Events
		pButton2->onPress.Add(			this,	& CGUIVolumeControlWidget::OnResetAlpha);
		pButtonX->onPress.Add(			this,	& CGUIVolumeControlWidget::OnSetXAxis);
		pButtonY->onPress.Add(			this,	& CGUIVolumeControlWidget::OnSetYAxis);
		pButtonZ->onPress.Add(			this,	& CGUIVolumeControlWidget::OnSetZAxis);
		VolumeMode->onSelection.Add(	this,	& CGUIVolumeControlWidget::OnVolumeMode);
	}
}

void CGUIVolumeControlWidget::resetVolumeRange()
{
	if (VolumeControl.Mode)
		GUIContext->getTitleLabels()->resetVolumeRangeIndicator(CProgramContext::get().DataManager);
}

void CGUIVolumeControlWidget::OnEmphasisSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.EmphasisLocation = Bar->GetValue();
	resetVolumeRange();
}

void CGUIVolumeControlWidget::OnIntensitySlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.AlphaIntensity = Bar->GetValue();
}

void CGUIVolumeControlWidget::OnMinimumAlphaSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.MinimumAlpha = Bar->GetValue();
}

void CGUIVolumeControlWidget::OnLocalRangeSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.LocalRange = Bar->GetValue();
	resetVolumeRange();
}

void CGUIVolumeControlWidget::OnResetVolume(Gwen::Controls::Base * Control)
{
	VolumeControl.Mode = 0;
}

void CGUIVolumeControlWidget::OnResetAlpha(Gwen::Controls::Base * Control)
{
	VolumeControl.AlphaIntensity = 1.f;
}

void CGUIVolumeControlWidget::OnSetXAxis(Gwen::Controls::Base * Control)
{
	VolumeControl.SliceAxis = SVector3f(1.f, 0.f, 0.f);
}

void CGUIVolumeControlWidget::OnSetYAxis(Gwen::Controls::Base * Control)
{
	VolumeControl.SliceAxis = SVector3f(0.f, 1.f, 0.f);
}

void CGUIVolumeControlWidget::OnSetZAxis(Gwen::Controls::Base * Control)
{
	VolumeControl.SliceAxis = SVector3f(0.f, 0.f, 1.f);
}

void CGUIVolumeControlWidget::OnVolumeMode(Gwen::Controls::Base * Control)
{
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Plane Slices"))
	{
		VolumeControl.Mode = 1;
		resetVolumeRange();
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Isosurface"))
	{
		VolumeControl.Mode = 2;
		resetVolumeRange();
	}
	else
	{
		VolumeControl.Mode = 0;
		CProgramContext::get().GUIContext->getTitleLabels()->clearVolumeRangeIndicator();
	}
}
