#include "CGUIManager.h"

#include <ionWindow.h>

#include <Gwen/Gwen.h>
#include <Gwen/Renderers/SFML.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Controls.h>
#include <Gwen/Controls/VerticalSlider.h>
#include <Gwen/Controls/HorizontalSlider.h>
#include <Gwen/Controls/ComboBox.h>

#include <iomanip>

#include "CMainState.h"
#include "CGwenEventForwarder.h"
#include "CVolumeControlsHandler.h"

#include "SciDataParser.h"


Gwen::Font * LoadFont(Gwen::UnicodeString const & File, float const Size)
{
	Gwen::Font * Font = new Gwen::Font();
	Font->facename = File;
	Font->size = Size;

	return Font;
}


CGUIManager::CGUIManager()
	: Application(CApplication::get()), MainState(CMainState::get()), ShowGUI(true)
{
}

void CGUIManager::init()
{
	Gwen::Renderer::SFML * pRenderer = new Gwen::Renderer::SFML(* Application.App);

	Gwen::Skin::TexturedBase * skin = new Gwen::Skin::TexturedBase();
	skin->SetRender(pRenderer);
	skin->Init("DefaultSkin.png");
	skin->SetDefaultFont(L"OpenSans.ttf", 14.f);
	LargeFont = LoadFont(L"OpenSans.ttf", 48.f);
	MediumFont = LoadFont(L"OpenSans.ttf", 32.f);
	RegularFont = LoadFont(L"OpenSans.ttf", 14.f);

	Canvas = new Gwen::Controls::Canvas(skin);
	Canvas->SetSize(Application.getWindowSize().X, Application.getWindowSize().Y);

	CGwenEventForwarder * Forwarder = new CGwenEventForwarder(getCanvas());
}

void CGUIManager::setup()
{
	Console = new CConsole(Canvas);

	// Top Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(Canvas);
	BigLabel->SetFont(LargeFont);
	BigLabel->SetText(Gwen::UnicodeString(L"Dataset: Hopavågen Bay - June 12, 2010 19:48 [T1]") + Gwen::UnicodeString());
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(235, 255, 235, 215));

	// Second Label
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Canvas);
	MediumLabel->SetFont(MediumFont);
	MediumLabel->SetText(Gwen::UnicodeString(L"Current Field: DO2 (microMol/L)") + Gwen::UnicodeString());
	MediumLabel->SetBounds(20, 70, 600, 300);
	MediumLabel->SetTextColor(Gwen::Color(235, 235, 255, 215));

	// Volume Range Label
	VolumeRangeIndicator = new Gwen::Controls::Label(Canvas);
	VolumeRangeIndicator->SetFont(MediumFont);
	VolumeRangeIndicator->SetBounds(20, 110, 600, 300);
	VolumeRangeIndicator->SetTextColor(Gwen::Color(255, 235, 235, 215));

	// Event Handler
	CVolumeControlsHandler * Handler = new CVolumeControlsHandler();
	
	// Slider Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Canvas);
		SliderLabel->SetFont(RegularFont);
		SliderLabel->SetText(L"Selection Value:");
		SliderLabel->SetBounds(1250, 10, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

		Gwen::Controls::HorizontalSlider * EmphasisSlider = new Gwen::Controls::HorizontalSlider(Canvas);
		EmphasisSlider->SetBounds(1250, 30, 300, 40);
		EmphasisSlider->SetRange(0.f, 1.f);

		SliderLabel = new Gwen::Controls::Label(Canvas);
		SliderLabel->SetFont(RegularFont);
		SliderLabel->SetText(L"Volume Opacity:");
		SliderLabel->SetBounds(1250, 70, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

		Gwen::Controls::HorizontalSlider * IntensitySlider = new Gwen::Controls::HorizontalSlider(Canvas);
		IntensitySlider->SetBounds(1250, 90, 300, 40);
		IntensitySlider->SetRange(10.f, 0.5f);

		SliderLabel = new Gwen::Controls::Label(Canvas);
		SliderLabel->SetFont(RegularFont);
		SliderLabel->SetText(L"Selection Range:");
		SliderLabel->SetBounds(1250, 130, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

		Gwen::Controls::HorizontalSlider * LocalRangeSlider = new Gwen::Controls::HorizontalSlider(Canvas);
		LocalRangeSlider->SetBounds(1250, 150, 300, 40);
		LocalRangeSlider->SetRange(0.05f, 0.5f);

		SliderLabel = new Gwen::Controls::Label(Canvas);
		SliderLabel->SetFont(RegularFont);
		SliderLabel->SetText(L"Selection Difference:");
		SliderLabel->SetBounds(1250, 190, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

		Gwen::Controls::HorizontalSlider * MinimumAlphaSlider = new Gwen::Controls::HorizontalSlider(Canvas);
		MinimumAlphaSlider->SetBounds(1250, 210, 300, 40);
		MinimumAlphaSlider->SetRange(0.0f, 0.5f);

		// Wire Up Events
		EmphasisSlider->onValueChanged.Add(		Handler,	& CVolumeControlsHandler::OnEmphasisSlider);
		IntensitySlider->onValueChanged.Add(	Handler,	& CVolumeControlsHandler::OnIntensitySlider);
		MinimumAlphaSlider->onValueChanged.Add(	Handler,	& CVolumeControlsHandler::OnMinimumAlphaSlider);
		LocalRangeSlider->onValueChanged.Add(	Handler,	& CVolumeControlsHandler::OnLocalRangeSlider);
	}

	
	// Other Controls Panel
	{
		Gwen::Controls::Label * ControlLabel = new Gwen::Controls::Label(Canvas);
		ControlLabel->SetFont(RegularFont);
		ControlLabel->SetText(L"Mode:");
		ControlLabel->SetBounds(1300, 320, 90, 40);
		ControlLabel->SetTextColor(Gwen::Color(255, 255, 200, 215));

		Gwen::Controls::ComboBox * VolumeMode = new Gwen::Controls::ComboBox(Canvas);
		VolumeMode->SetBounds(1350, 120 + 120 + 45 + 35, 200, 25);
		VolumeMode->AddItem(L"Full Volume");
		VolumeMode->AddItem(L"Plane Slices");
		VolumeMode->AddItem(L"Isosurface");

		Gwen::Controls::Button * pButton2 = new Gwen::Controls::Button(Canvas);
		pButton2->SetBounds(1350, 120 + 120 + 10 + 35, 200, 25);
		pButton2->SetText("Reset Alpha Intensity");
	
		ControlLabel = new Gwen::Controls::Label(Canvas);
		ControlLabel->SetFont(RegularFont);
		ControlLabel->SetText(L"Axis:");
		ControlLabel->SetBounds(1313, 355, 90, 40);
		ControlLabel->SetTextColor(Gwen::Color(255, 255, 200, 215));

		Gwen::Controls::Button * pButtonX = new Gwen::Controls::Button(Canvas);
		pButtonX->SetBounds(1350, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
		pButtonX->SetText("X");

		Gwen::Controls::Button * pButtonY = new Gwen::Controls::Button(Canvas);
		pButtonY->SetBounds(1400, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
		pButtonY->SetText("Y");

		Gwen::Controls::Button * pButtonZ = new Gwen::Controls::Button(Canvas);
		pButtonZ->SetBounds(1450, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
		pButtonZ->SetText("Z");

		// Wire Up Events
		pButton2->onPress.Add(			Handler,	& CVolumeControlsHandler::OnResetAlpha);
		pButtonX->onPress.Add(			Handler,	& CVolumeControlsHandler::OnSetXAxis);
		pButtonY->onPress.Add(			Handler,	& CVolumeControlsHandler::OnSetYAxis);
		pButtonZ->onPress.Add(			Handler,	& CVolumeControlsHandler::OnSetZAxis);
		VolumeMode->onSelection.Add(	Handler,	& CVolumeControlsHandler::OnVolumeMode);
	}


	Console->addMessage("GUI Initialized.");
	Console->addMessage("Starting program...", Gwen::Colors::Red);
}

void CGUIManager::resetVolumeRangeIndicator(SciDataParser * DataParser)
{
	Range ValueRange = DataParser->GridValues.getValueRange("o1", 5.0);
	std::wstringstream s;
	s << std::fixed;
	s << "Value Range: ";
	s << std::setprecision(1);
	s << (MainState.VolumeSceneObject.Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first) / 100.f;
	s << " ± ";
	s << std::setprecision(2);
	s << (MainState.VolumeSceneObject.Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first)) / 100.f;
	VolumeRangeIndicator->SetText(s.str());
}

void CGUIManager::clearVolumeRangeIndicator()
{
	VolumeRangeIndicator->SetText(L"");
}
