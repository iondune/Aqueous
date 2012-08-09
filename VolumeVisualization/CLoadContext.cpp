#include "CLoadContext.h"

#include "CMainState.h"


void CLoadContext::addLabel(std::wstring const & Label)
{
	Gwen::Controls::Label * MediumLabel = new Gwen::Controls::Label(Canvas);
	MediumLabel->SetFont(GUIManager->getMediumFont());
	MediumLabel->SetText(Label);
	MediumLabel->SetBounds(20, LabelHeight, 600, 300);
	MediumLabel->SetTextColor(Gwen::Color(255, 255, 255, 84));

	GUIManager->draw(true);

	LabelHeight += 40;
}

CLoadContext::CLoadContext()
	: LabelHeight(0)
{}

void CLoadContext::init()
{
	loadContext();

	LabelHeight = 70;
}

void CLoadContext::run()
{
	// Load References
	CApplication & Application = CApplication::get();
	CMainState & MainState = CMainState::get();

	GUIManager = Context->GUIManager;
	Canvas = GUIManager->getCanvas();
	
	// Init Canvas
	Canvas->SetBackgroundColor(Gwen::Color(32, 48, 48));
	Canvas->SetDrawBackground(true);

	// Label 1
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(Canvas);
	BigLabel->SetFont(GUIManager->getLargeFont());
	BigLabel->SetText(Gwen::UnicodeString(L"Loading..."));
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(255, 255, 255, 84));
	GUIManager->draw(true);

	addLabel(L"Initializing System...");
	Application.loadEngines();
	MainState.loadEngineReferences();

	addLabel(L"Loading Scene Shaders...");
	Application.getSceneManager().init();
	
	addLabel(L"Loading Science Data...");
	loadData();
}
