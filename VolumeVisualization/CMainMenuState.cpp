
#include "CMainMenuState.h"

#include <ionGUI.h>

#include "CMainState.h"
#include "ColorMappers.h"
#include "CDataLoadingThread.h"
#include "CGlyphNodeManager.h"
#include "CGUIContext.h"

#include "CSite.h"


CMainMenuState::CMainMenuState()
{}

void CMainMenuState::Begin()
{
	Context->GUIContext->GetCanvas()->SetDrawBackground(true);
	CGUIMainMenuWidget * MainMenu = new CGUIMainMenuWidget();
	std::cout << "Menu State begin." << std::endl;
}

void CMainMenuState::End()
{
	Context->GUIContext->Clear();
	std::cout << "Menu State end." << std::endl;
}

void CMainMenuState::Update(f32 const Elapsed)
{
	std::chrono::milliseconds Milliseconds(1);
	std::this_thread::sleep_for(Milliseconds);

	Thread.Sync();

	Context->GUIContext->Manager->Draw(Elapsed, true);
}

void CMainMenuState::OnEvent(SWindowResizedEvent & Event)
{
	Context->GUIContext->GetCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->GetCanvas()->Invalidate();
	Context->GUIContext->GetCanvas()->InvalidateChildren(true);
}

void CMainMenuState::LoadData()
{
	Thread.Context = Context;
	Thread.LoadingWidget = new CGUIProgressBarWidget(Context->GUIContext->Manager.Get(), "Loading data and initializing scene elements");
	Thread.LoadingWidget->BeginProgress();
	Thread.Run();
}

void CMainMenuState::CreateDataSet()
{
	//SciDataParserCSV Parser1;
	//CSpectrumColorMapper ColorMapper("Chla Conc");

	//Parser1.DataSet = Context->CurrentSite->GetCurrentDataSet();
	//Parser1.FieldDelim = Parser1.ValueDelim = ';';
	////Parser1.Load("Sites/Catalina/20131006_130020_catalina_shallow_10_IVER2-132.log");
	////Parser1.Load("Sites/Catalina/20131005_191049_catalina_10_IVER2-132.log"); // BAD!
	//Parser1.Load("Sites/Catalina/20131006_120459_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_121530_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_122601_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_124220_catalina_10_IVER2-132.log");
	////Parser1.Load("Sites/Catalina/20131006_130020_catalina_shallow_10_IVER2-132.log");
	////Parser1.Load("Sites/Catalina/20131006_142755_catalina_undulate_10_IVER2-132.log");
	////Parser1.Load("Sites/Catalina/20131006_143331_catalina_undulate_10_IVER2-132.log");
	////Parser1.Load("Sites/Catalina/20131006_221316_catalina_10_IVER2-132.log"); // BAD!
	//Parser1.Load("Sites/Catalina/20131006_221639_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_222457_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_222608_catalina_10_IVER2-132.log");
	////Parser1.Load("Sites/Catalina/20131006_222654_catalina_10_IVER2-132.log"); // Out of range
	////Parser1.Load("Sites/Catalina/20131006_223043_catalina_10_IVER2-132.log"); // Out of range
	//Parser1.Load("Sites/Catalina/20131007_100031_catalina_10_IVER2-132.log");

	//Parser1.Load("Sites/Catalina/20131005_191049_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_120459_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_121530_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_122601_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_124220_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_130020_catalina_shallow_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_142755_catalina_undulate_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_143331_catalina_undulate_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_221316_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_221639_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_222457_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_222608_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_222654_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131006_223043_catalina_10_IVER2-132.log");
	//Parser1.Load("Sites/Catalina/20131007_100031_catalina_10_IVER2-132.log");

	//Context->DataManager->createGridDataFromRawValuesRBFI(SRange<f64>::Full, 15.0, "Chla Conc");
	//Context->DataManager->createVolumeFromGridValues(& ColorMapper);
	//Context->DataManager->writeToFile("Datasets/DenmarkNewMission1.dat");

	
	/*
	SciDataParserSimpleTXT * Parser1 = new SciDataParserSimpleTXT();
	Parser1->Manager = Context->DataManager;
	Parser1->load("ForZoe.txt");

	SciDataParserGrid1 * Parser2 = new SciDataParserGrid1();
	Parser2->Manager = Context->DataManager;
	Parser2->load("oxyMaps.mat");

	COxygenColorMapper o;
	Context->DataManager->createVolumeFromGridValues(& o);
	Context->DataManager->writeToFile("Datasets/Catalina1.dat");*/

	//COxygenColorMapper o;
	//Context->CurrentSite->GetCurrentDataSet()->createVolumeFromGridValues(& o);
	//Context->DataManager->writeToFile("Datasets/HopavagenBay1.dat");
}
