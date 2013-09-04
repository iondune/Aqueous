
#include "CMainMenuState.h"

#include "CMainState.h"
#include "ColorMappers.h"
#include "CGUILoadingWidget.h"
#include "CDataLoadingThread.h"
#include "SciDataManager.h"
#include "CGlyphSceneObject.h"


CMainMenuState::CMainMenuState()
{}

void CMainMenuState::Begin()
{
	Context->GUIContext->SetupMenuState();
	std::cout << "Menu State begin." << std::endl;
}

void CMainMenuState::End()
{
	Context->GUIContext->Clear();
	std::cout << "Menu State end." << std::endl;
}

void CMainMenuState::Update(f32 const Elapsed)
{
	printOpenGLErrors();
	// Let loading thread run
	//sf::sleep(sf::seconds(0.05f));
	std::chrono::milliseconds Milliseconds(50);
	std::this_thread::sleep_for(Milliseconds);

	Thread.Sync();

	Context->GUIContext->Draw(Elapsed, true);
	CApplication::Get().GetWindow().SwapBuffers();

	static int counter = 0;
	
	if (! counter--)
	{
		createDataSet();
		loadData(//"DenmarkMission1.dat");
			"HopavagenBayNew1.dat");
	}

	//loaded = true;
}

void CMainMenuState::OnEvent(SWindowResizedEvent & Event)
{
	Context->GUIContext->GetCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->GetCanvas()->Invalidate();
	Context->GUIContext->GetCanvas()->InvalidateChildren(true);
}

void CMainMenuState::loadData(std::string const & FileName)
{
	DataSetName = FileName;

	std::stringstream s;
	s << "Datasets/";
	s << FileName;

	Thread.Context = Context;
	Context->GUIContext->AddWidget(Thread.LoadingWidget = new CGUILoadingWidget("Loading data and initializing scene elements"));
	Thread.Run(s.str());
}

void CMainMenuState::createDataSet()
{
	SciDataParserSimpleTXT * Parser1 = new SciDataParserSimpleTXT();
	Parser1->Manager = Context->DataManager;
	Parser1->load("ForZoe.txt");

	SciDataParserGrid1 * Parser2 = new SciDataParserGrid1();
	Parser2->Manager = Context->DataManager;
	Parser2->load("oxyMaps.mat");

	COxygenColorMapper o;
	Context->DataManager->createVolumeFromGridValues(& o);
	Context->DataManager->writeToFile("Datasets/HopavagenBay1.dat");
}
