#include "CMainState.h"

#include <Gwen/Gwen.h>
#include <Gwen/Renderers/SFML.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Controls.h>
#include <Gwen/Controls/VerticalSlider.h>
#include <Gwen/Controls/HorizontalSlider.h>
#include <Gwen/Controls/ComboBox.h>

#include "CGwenEventForwarder.h"
#include <CApplication.h>

#include "CVolumeControlsHandler.h"


CMainState::CMainState()
	: Camera(0), Scale(1), Mode(0), BindLightPosition(LightPosition),
	ShowGUI(true), Slider(0.f)
{
	DataParser[0] = DataParser[1] = DataParser[2] = 0;
}

Gwen::Font * LoadFont(Gwen::UnicodeString const & File, float const Size)
{
	Gwen::Font * Font = new Gwen::Font();
	Font->facename = File;
	Font->size = Size;

	return Font;
}

SVector3f LightPosition;

void CMainState::begin()
{
	init();
	initScene();

	loadData();

	// setup GWEN
	Gwen::Renderer::SFML * pRenderer = new Gwen::Renderer::SFML(* CApplication::get().App);

	Gwen::Skin::TexturedBase * skin = new Gwen::Skin::TexturedBase(/*pRenderer*/);
	skin->SetRender(pRenderer);
	skin->Init("DefaultSkin.png");
	skin->SetDefaultFont(L"OpenSans.ttf", 14.f);
	Gwen::Font * LargeFont = LoadFont(L"OpenSans.ttf", 48.f);
	Gwen::Font * MediumFont = LoadFont(L"OpenSans.ttf", 32.f);
	Gwen::Font * RegularFont = LoadFont(L"OpenSans.ttf", 14.f);

	pCanvas = new Gwen::Controls::Canvas(skin);
	pCanvas->SetSize(Application.getWindowSize().X, Application.getWindowSize().Y);

	Console = new CConsole(pCanvas);

	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(pCanvas);
	BigLabel->SetFont(LargeFont);
	BigLabel->SetText(Gwen::UnicodeString(L"Dataset: Hopavågen Bay - June 12, 2010 19:48 [T1]") + Gwen::UnicodeString());
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(235, 255, 235, 215));

	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(pCanvas);
	MediumLabel->SetFont(MediumFont);
	MediumLabel->SetText(Gwen::UnicodeString(L"Current Field: DO2 (microMol/L)") + Gwen::UnicodeString());
	MediumLabel->SetBounds(20, 70, 600, 300);
	MediumLabel->SetTextColor(Gwen::Color(235, 235, 255, 215));

	VolumeRangeIndicator = new Gwen::Controls::Label(pCanvas);
	VolumeRangeIndicator->SetFont(MediumFont);
	VolumeRangeIndicator->SetBounds(20, 110, 600, 300);
	VolumeRangeIndicator->SetTextColor(Gwen::Color(255, 235, 235, 215));

	

	Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(pCanvas);
	SliderLabel->SetFont(RegularFont);
	SliderLabel->SetText(L"Selection Value:");
	SliderLabel->SetBounds(1250, 10, 300, 40);
	SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

	Gwen::Controls::HorizontalSlider * EmphasisSlider = new Gwen::Controls::HorizontalSlider(pCanvas);
	EmphasisSlider->SetBounds(1250, 30, 300, 40);
	EmphasisSlider->SetRange(0.f, 1.f);

	SliderLabel = new Gwen::Controls::Label(pCanvas);
	SliderLabel->SetFont(RegularFont);
	SliderLabel->SetText(L"Volume Opacity:");
	SliderLabel->SetBounds(1250, 70, 300, 40);
	SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

	Gwen::Controls::HorizontalSlider * IntensitySlider = new Gwen::Controls::HorizontalSlider(pCanvas);
	IntensitySlider->SetBounds(1250, 90, 300, 40);
	IntensitySlider->SetRange(10.f, 0.5f);

	SliderLabel = new Gwen::Controls::Label(pCanvas);
	SliderLabel->SetFont(RegularFont);
	SliderLabel->SetText(L"Selection Range:");
	SliderLabel->SetBounds(1250, 130, 300, 40);
	SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

	Gwen::Controls::HorizontalSlider * LocalRangeSlider = new Gwen::Controls::HorizontalSlider(pCanvas);
	LocalRangeSlider->SetBounds(1250, 150, 300, 40);
	LocalRangeSlider->SetRange(0.05f, 0.5f);

	SliderLabel = new Gwen::Controls::Label(pCanvas);
	SliderLabel->SetFont(RegularFont);
	SliderLabel->SetText(L"Selection Difference:");
	SliderLabel->SetBounds(1250, 190, 300, 40);
	SliderLabel->SetTextColor(Gwen::Color(255, 200, 200, 215));

	Gwen::Controls::HorizontalSlider * MinimumAlphaSlider = new Gwen::Controls::HorizontalSlider(pCanvas);
	MinimumAlphaSlider->SetBounds(1250, 210, 300, 40);
	MinimumAlphaSlider->SetRange(0.0f, 0.5f);


	

	SliderLabel = new Gwen::Controls::Label(pCanvas);
	SliderLabel->SetFont(RegularFont);
	SliderLabel->SetText(L"Mode:");
	SliderLabel->SetBounds(1300, 320, 90, 40);
	SliderLabel->SetTextColor(Gwen::Color(255, 255, 200, 215));

	Gwen::Controls::ComboBox * VolumeMode = new Gwen::Controls::ComboBox(pCanvas);
	VolumeMode->SetBounds(1350, 120 + 120 + 45 + 35, 200, 25);
	VolumeMode->AddItem(L"Full Volume");
	VolumeMode->AddItem(L"Plane Slices");
	VolumeMode->AddItem(L"Isosurface");



	Gwen::Controls::Button * pButton2 = new Gwen::Controls::Button(pCanvas);
	pButton2->SetBounds(1350, 120 + 120 + 10 + 35, 200, 25);
	pButton2->SetText("Reset Alpha Intensity");

	
	SliderLabel = new Gwen::Controls::Label(pCanvas);
	SliderLabel->SetFont(RegularFont);
	SliderLabel->SetText(L"Axis:");
	SliderLabel->SetBounds(1313, 355, 90, 40);
	SliderLabel->SetTextColor(Gwen::Color(255, 255, 200, 215));

	Gwen::Controls::Button * pButtonX = new Gwen::Controls::Button(pCanvas);
	pButtonX->SetBounds(1350, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
	pButtonX->SetText("X");

	Gwen::Controls::Button * pButtonY = new Gwen::Controls::Button(pCanvas);
	pButtonY->SetBounds(1400, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
	pButtonY->SetText("Y");

	Gwen::Controls::Button * pButtonZ = new Gwen::Controls::Button(pCanvas);
	pButtonZ->SetBounds(1450, 120 + 120 + 10 + 45 + 25 + 35, 40, 25);
	pButtonZ->SetText("Z");




	CVolumeControlsHandler * Handler = new CVolumeControlsHandler();
	EmphasisSlider->onValueChanged.Add(		Handler,	& CVolumeControlsHandler::OnEmphasisSlider);
	IntensitySlider->onValueChanged.Add(	Handler,	& CVolumeControlsHandler::OnIntensitySlider);
	MinimumAlphaSlider->onValueChanged.Add(	Handler,	& CVolumeControlsHandler::OnMinimumAlphaSlider);
	LocalRangeSlider->onValueChanged.Add(	Handler,	& CVolumeControlsHandler::OnLocalRangeSlider);
	pButton2->onPress.Add(					Handler,	& CVolumeControlsHandler::OnResetAlpha);
	pButtonX->onPress.Add(					Handler,	& CVolumeControlsHandler::OnSetXAxis);
	pButtonY->onPress.Add(					Handler,	& CVolumeControlsHandler::OnSetYAxis);
	pButtonZ->onPress.Add(					Handler,	& CVolumeControlsHandler::OnSetZAxis);
	VolumeMode->onSelection.Add(			Handler,	& CVolumeControlsHandler::OnVolumeMode);

	addConsoleMessage("GUI Initialized.");
	addConsoleMessage("Starting program...", Gwen::Colors::Red);

	CGwenEventForwarder * Forwarder = new CGwenEventForwarder(pCanvas);

	addConsoleMessage("Volume mesh created.", Gwen::Color(0, 255, 0));

	Timer = 0.f;
}

#include <iomanip>

void CMainState::resetVolumeRangeIndicator()
{
	Range ValueRange = DataParser[2]->GridValues.getValueRange("o1", 5.0);
	std::wstringstream s;
	s << std::fixed;
	s << "Value Range: ";
	s << std::setprecision(1);
	s << (VolumeSceneObject.Control.EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first) / 100.f;
	s << " ± ";
	s << std::setprecision(2);
	s << (VolumeSceneObject.Control.LocalRange / 2.f * (ValueRange.second - ValueRange.first)) / 100.f;
	VolumeRangeIndicator->SetText(s.str());
}

void CMainState::clearVolumeRangeIndicator()
{
	VolumeRangeIndicator->SetText(L"");
}

void CMainState::OnRenderStart(float const Elapsed)
{
	Camera->update(Elapsed);

	Timer += Elapsed * 0.16f;

	float const Distance = 4.f;
	OrbitCamera->setPosition(SVector3f(sin(Timer)*Distance, 2.3f, cos(Timer)*Distance));
	OrbitCamera->setLookAtTarget(SVector3f());

	::LightPosition = LightPosition = SceneManager.getActiveCamera()->getPosition() + SVector3f(0, 0, 0);

	LightObject->setTranslation(LightPosition);
	

	SceneManager.drawAll();
	SceneManager.endDraw();


	
	glBindTexture(GL_TEXTURE_2D, 0);
	

	if (ShowGUI)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glClear(GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		int left = 0, top = 0;
		int right = 1600, bottom = 900;
		glOrtho( left, right, bottom, top, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(0, 0, right - left, bottom - top);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		Console->update(Elapsed);


		pCanvas->RenderCanvas();
	}

	CApplication::get().swapBuffers();
	Terrain->DoCameraUpdate = false;
}

void CMainState::addConsoleMessage(std::string const & Message, Gwen::Color const & Color)
{
	Console->addMessage(Message, Color);
}
