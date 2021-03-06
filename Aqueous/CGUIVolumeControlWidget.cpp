#include "CGUIVolumeControlWidget.h"

#include "CProgramContext.h"
#include "CGUIContext.h"
#include "CMainState.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUIVolumeControlWidget::CGUIVolumeControlWidget()
	: VolumeControl(CProgramContext::Get().Scene.Volume->Control), MainState(CMainState::Get())
{
	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 10, 330, 620);
	Window->SetTitle("Volume Controls");
	Window->SetHidden(true);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText("Disable Volume View");
	EnableButton->onPress.Add(this, & CGUIVolumeControlWidget::OnToggleVolume);

	// Slider Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Selection Value:");
		SliderLabel->SetBounds(10, 10 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSlider * EmphasisSlider = new Gwen::Controls::HorizontalSlider(Window);
		EmphasisSlider->SetBounds(10, 30 + 45, 300, 40);
		EmphasisSlider->SetRange(0.f, 1.f);
		EmphasisSlider->SetFloatValue(0.5f);

		SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Volume Opacity:");
		SliderLabel->SetBounds(10, 70 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		IntensitySlider = new Gwen::Controls::HorizontalSlider(Window);
		IntensitySlider->SetBounds(10, 90 + 45, 300, 40);
		IntensitySlider->SetRange(0.5f, 10.f);
		IntensitySlider->SetFloatValue(1.f);

		SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Selection Range:");
		SliderLabel->SetBounds(10, 130 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSlider * LocalRangeSlider = new Gwen::Controls::HorizontalSlider(Window);
		LocalRangeSlider->SetBounds(10, 150 + 45, 300, 40);
		LocalRangeSlider->SetRange(0.05f, 0.5f);
		LocalRangeSlider->SetFloatValue(0.1f);

		SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Selection Difference:");
		SliderLabel->SetBounds(10, 190 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSlider * MinimumAlphaSlider = new Gwen::Controls::HorizontalSlider(Window);
		MinimumAlphaSlider->SetBounds(10, 210 + 45, 300, 40);
		MinimumAlphaSlider->SetRange(0.0f, 0.5f);
		MinimumAlphaSlider->SetFloatValue(0.1f);

		// Wire Up Events
		EmphasisSlider->onValueChanged.Add(this,		& CGUIVolumeControlWidget::OnEmphasisSlider);
		IntensitySlider->onValueChanged.Add(this,		& CGUIVolumeControlWidget::OnIntensitySlider);
		MinimumAlphaSlider->onValueChanged.Add(this,	& CGUIVolumeControlWidget::OnMinimumAlphaSlider);
		LocalRangeSlider->onValueChanged.Add(this,		& CGUIVolumeControlWidget::OnLocalRangeSlider);
	}

	
	// Other Controls Panel
	{
		Gwen::Controls::Label * ControlLabel = new Gwen::Controls::Label(Window);
		ControlLabel->SetFont(GUIManager->GetRegularFont());
		ControlLabel->SetText(L"Mode:");
		ControlLabel->SetBounds(30, 320 + 45 + 6, 90, 40);
		ControlLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));
		
		Gwen::Controls::Label * DebugLabel = new Gwen::Controls::Label(Window);
		DebugLabel->SetFont(GUIManager->GetRegularFont());
		DebugLabel->SetText(L"Debug:");
		DebugLabel->SetBounds(22, 320 + 45 + 70 + 6, 90, 40);
		DebugLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::Label * ShadingLabel = new Gwen::Controls::Label(Window);
		ShadingLabel->SetFont(GUIManager->GetRegularFont());
		ShadingLabel->SetText(L"Shading:");
		ShadingLabel->SetBounds(10, 320 + 45 + 70 + 35 + 6, 90, 40);
		ShadingLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::Button * pButton2 = new Gwen::Controls::Button(Window);
		pButton2->SetBounds(80, 120 + 120 + 10 + 35 + 45, 200, 25);
		pButton2->SetText("Reset Volume Opacity");
		
		Gwen::Controls::ComboBox * VolumeMode = new Gwen::Controls::ComboBox(Window);
		VolumeMode->SetBounds(80, 120 + 120 + 45 + 35 + 45, 200, 25);
		VolumeMode->AddItem(L"Constant");
		VolumeMode->AddItem(L"Sample Alpha");
		VolumeMode->AddItem(L"Plane Slices");
		VolumeMode->AddItem(L"Isosurface");

		Gwen::Controls::ComboBox * DebugMode = new Gwen::Controls::ComboBox(Window);
		DebugMode->SetBounds(80, 120 + 120 + 45 + 35 + 45 + 70, 100, 25);
		DebugMode->AddItem(L"Disabled");
		DebugMode->AddItem(L"Front");
		DebugMode->AddItem(L"Cutoff");
		DebugMode->AddItem(L"Accumulator");
		DebugMode->AddItem(L"Iterations");
		DebugMode->AddItem(L"Box Depth");
		DebugMode->AddItem(L"Gradient");

		Gwen::Controls::ComboBox * ShadingMode = new Gwen::Controls::ComboBox(Window);
		ShadingMode->SetBounds(80, 120 + 120 + 45 + 35 + 45 + 70 + 35, 200, 25);
		ShadingMode->AddItem(L"No Shading");
		ShadingMode->AddItem(L"Orbit Camera");
		ShadingMode->AddItem(L"Fly Camera");

		Gwen::Controls::Label * DepthLabel = new Gwen::Controls::Label(Window);
		DepthLabel->SetFont(GUIManager->GetRegularFont());
		DepthLabel->SetText(L"Depth:");
		DepthLabel->SetBounds(80 + 100 + 10, 320 + 45 + 70 + 6, 90, 40);
		DepthLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::CheckBox * DepthMode = new Gwen::Controls::CheckBox(Window);
		DepthMode->SetBounds(80 + 100 + 10 + 60, 120 + 120 + 45 + 35 + 45 + 70 + 6, 15, 15);
	
		ControlLabel = new Gwen::Controls::Label(Window);
		ControlLabel->SetFont(GUIManager->GetRegularFont());
		ControlLabel->SetText(L"Axis:");
		ControlLabel->SetBounds(43, 355 + 45 + 6, 90, 40);
		ControlLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::Button * pButtonX = new Gwen::Controls::Button(Window);
		pButtonX->SetBounds(80, 120 + 120 + 10 + 45 + 25 + 35 + 45, 40, 25);
		pButtonX->SetText("X");

		Gwen::Controls::Button * pButtonY = new Gwen::Controls::Button(Window);
		pButtonY->SetBounds(130, 120 + 120 + 10 + 45 + 25 + 35 + 45, 40, 25);
		pButtonY->SetText("Y");

		Gwen::Controls::Button * pButtonZ = new Gwen::Controls::Button(Window);
		pButtonZ->SetBounds(180, 120 + 120 + 10 + 45 + 25 + 35 + 45, 40, 25);
		pButtonZ->SetText("Z");

		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Volume Detail:");
		SliderLabel->SetBounds(10, 530, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSlider * StepSizeSlider = new Gwen::Controls::HorizontalSlider(Window);
		StepSizeSlider->SetBounds(10, 545, 300, 40);
		StepSizeSlider->SetRange(10.f, 300.f);
		//StepSizeSlider->SetFloatValue(VolumeControl.StepSize);

		// Wire Up Events
		pButton2->onPress.Add(this,					& CGUIVolumeControlWidget::OnResetAlpha);
		pButtonX->onPress.Add(this,					& CGUIVolumeControlWidget::OnSetXAxis);
		pButtonY->onPress.Add(this,					& CGUIVolumeControlWidget::OnSetYAxis);
		pButtonZ->onPress.Add(this,					& CGUIVolumeControlWidget::OnSetZAxis);
		VolumeMode->onSelection.Add(this,			& CGUIVolumeControlWidget::OnVolumeMode);
		DebugMode->onSelection.Add(this,			& CGUIVolumeControlWidget::OnDebugMode);
		ShadingMode->onSelection.Add(this,			& CGUIVolumeControlWidget::OnShadingMode);
		StepSizeSlider->onValueChanged.Add(this,	& CGUIVolumeControlWidget::OnStepSizeSlider);
		DepthMode->onCheckChanged.Add(this,			& CGUIVolumeControlWidget::OnDepthMode);
	}
}

void CGUIVolumeControlWidget::resetVolumeRange()
{
	if (VolumeControl.Mode)
		GUIContext->GetTitleLabels()->resetVolumeRangeIndicator(CProgramContext::Get().CurrentSite);
}

void CGUIVolumeControlWidget::OnEmphasisSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.EmphasisLocation = Bar->GetFloatValue();
	resetVolumeRange();
}

void CGUIVolumeControlWidget::OnIntensitySlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.AlphaIntensity = Bar->GetFloatValue();
}

void CGUIVolumeControlWidget::OnMinimumAlphaSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.MinimumAlpha = Bar->GetFloatValue();
}

void CGUIVolumeControlWidget::OnLocalRangeSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.LocalRange = Bar->GetFloatValue();
	resetVolumeRange();
}

void CGUIVolumeControlWidget::OnStepSizeSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.StepSize = Bar->GetFloatValue();
}

void CGUIVolumeControlWidget::OnResetVolume(Gwen::Controls::Base * Control)
{
	VolumeControl.Mode = 0;
}

void CGUIVolumeControlWidget::OnResetAlpha(Gwen::Controls::Base * Control)
{
	IntensitySlider->SetFloatValue(1.f);
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
		CProgramContext::Get().GUIContext->GetTitleLabels()->clearVolumeRangeIndicator();
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Isosurface"))
	{
		VolumeControl.Mode = 2;
		resetVolumeRange();
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Constant"))
	{
		VolumeControl.Mode = 0;
		CProgramContext::Get().GUIContext->GetTitleLabels()->clearVolumeRangeIndicator();
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Sample Alpha"))
	{
		VolumeControl.Mode = 3;
		CProgramContext::Get().GUIContext->GetTitleLabels()->clearVolumeRangeIndicator();
	}
}

void CGUIVolumeControlWidget::OnShadingMode(Gwen::Controls::Base * Control)
{
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"No Shading"))
		VolumeControl.UseShading = 0;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Orbit Camera"))
		VolumeControl.UseShading = 1;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Fly Camera"))
		VolumeControl.UseShading = 2;
}

void CGUIVolumeControlWidget::OnDebugMode(Gwen::Controls::Base * Control)
{
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Disabled"))
		VolumeControl.DebugLevel = 0;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Front"))
		VolumeControl.DebugLevel = 1;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Cutoff"))
		VolumeControl.DebugLevel = 2;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Accumulator"))
		VolumeControl.DebugLevel = 3;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Iterations"))
		VolumeControl.DebugLevel = 4;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Box Depth"))
		VolumeControl.DebugLevel = 5;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Gradient"))
		VolumeControl.DebugLevel = 6;
}

void CGUIVolumeControlWidget::OnDepthMode(Gwen::Controls::Base * Control)
{
	Gwen::Controls::CheckBox * Box = (Gwen::Controls::CheckBox *) Control;

	CMainState::Get().ShowDepth = Box->IsChecked();
}

void CGUIVolumeControlWidget::OnToggleVolume(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	if (Context->Scene.Volume->GetNode()->IsVisible())
	{
		Context->Scene.Volume->GetNode()->SetVisible(false);
		GUIContext->GetConsole()->AddMessage("Volume View Disabled");
		EnableButton->SetText("Enable Volume View");
	}
	else
	{
		Context->Scene.Volume->GetNode()->SetVisible(true);
		GUIContext->GetConsole()->AddMessage("Volume View Enabled");
		EnableButton->SetText("Disable Volume View");
	}
}

void CGUIVolumeControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
