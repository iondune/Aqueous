
#include "CMainMenuState.h"

#include <ionGUI.h>

#include "CMainState.h"
#include "ColorMappers.h"
#include "CDataLoadingThread.h"
#include "CGlyphSceneObject.h"

#include "SciDataManager.h"
#include "SciDataParser.h"


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
	std::chrono::milliseconds Milliseconds(50);
	std::this_thread::sleep_for(Milliseconds);

	Thread.Sync();

	Context->GUIContext->Draw(Elapsed, true);
	CApplication::Get().GetWindow().SwapBuffers();

	static int Counter = 0;
	if (! Counter--)
	{
		Context->DataManager->GetRawValues().Traits.PositionXField = "Longitude";
		Context->DataManager->GetRawValues().Traits.PositionYField = "DFS Depth (m)";
		Context->DataManager->GetRawValues().Traits.PositionZField = "Latitude";

		//CreateDataSet();
		LoadData("DenmarkNewMission1.dat");
	}
}

void CMainMenuState::OnEvent(SWindowResizedEvent & Event)
{
	Context->GUIContext->GetCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->GetCanvas()->Invalidate();
	Context->GUIContext->GetCanvas()->InvalidateChildren(true);
}

void CMainMenuState::LoadData(std::string const & FileName)
{
	DataSetName = FileName;

	std::stringstream s;
	s << "Datasets/";
	s << FileName;

	Thread.Context = Context;
	Thread.LoadingWidget = new CGUIProgressBarWidget(Context->GUIContext, "Loading data and initializing scene elements");
	Thread.LoadingWidget->BeginProgress();
	Thread.Run(s.str());
}

void CMainMenuState::CreateDataSet()
{
	SciDataParserCSV Parser1;
	CSpectrumColorMapper ColorMapper("Chla Conc");

	Parser1.Manager = Context->DataManager;
	Parser1.Load("Sites/Denmark/mission1.csv");

	Context->DataManager->createGridDataFromRawValuesRBFI(FullRange, 15.0, "Chla Conc");
	Context->DataManager->createVolumeFromGridValues(& ColorMapper);
	Context->DataManager->writeToFile("Datasets/DenmarkNewMission1.dat");

	/*
	SciDataParserSimpleTXT * Parser1 = new SciDataParserSimpleTXT();
	Parser1->Manager = Context->DataManager;
	Parser1->load("ForZoe.txt");

	SciDataParserGrid1 * Parser2 = new SciDataParserGrid1();
	Parser2->Manager = Context->DataManager;
	Parser2->load("oxyMaps.mat");

	COxygenColorMapper o;
	Context->DataManager->createVolumeFromGridValues(& o);
	Context->DataManager->writeToFile("Datasets/HopavagenBay1.dat");
	*/
}
