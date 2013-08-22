#include "CMainMenuState.h"

#include "CMainState.h"
#include "ColorMappers.h"
#include "CGUILoadingWidget.h"
#include "CDataLoadingThread.h"
#include "SciDataManager.h"


CMainMenuState::CMainMenuState()
{}

void CMainMenuState::Begin()
{
	Context->GUIContext->setupMenuState();
	std::cout << "Menu State begin." << std::endl;
}

void CMainMenuState::End()
{
	Context->GUIContext->clear();
	std::cout << "Menu State end." << std::endl;
}

#include "CGlyphSceneObject.h"

void CMainMenuState::Update(f32 const Elapsed)
{
	printOpenGLErrors();
	// Let loading thread run
	//sf::sleep(sf::seconds(0.05f));
	std::chrono::milliseconds Milliseconds(50);
	std::this_thread::sleep_for(Milliseconds);

	Thread.Sync();

	Context->GUIContext->draw(Elapsed, true);
	CApplication::Get().GetWindow().SwapBuffers();

	static int counter = 0;
	
	if (! counter--)
	{
		//createDataSet();
		loadData(//"DenmarkMission1.dat");
			"HopavagenBay1.dat");
	}

	//loaded = true;
}

void CMainMenuState::OnEvent(SWindowResizedEvent & Event)
{
	Context->GUIContext->getCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->getCanvas()->Invalidate();
	Context->GUIContext->getCanvas()->InvalidateChildren(true);
}

void CMainMenuState::loadData(std::string const & FileName)
{
	DataSetName = FileName;

	std::stringstream s;
	s << "Datasets/";
	s << FileName;

	Thread.Context = Context;
	Context->GUIContext->addWidget(Thread.LoadingWidget = new CGUILoadingWidget("Loading data and initializing scene elements"));
	Thread.Run(s.str());
}

void CMainMenuState::createDataSet()
{
	if (false)
	{
		SciDataParserSimpleTXT * Parser = new SciDataParserSimpleTXT();
		Parser->Manager = Context->DataManager;
		Parser->load("ForZoe.txt");

		//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));
		
		CSpectrumColorMapper sf("d1");
		COxygenColorMapper o("d1");
		sf.AcceptedRange = Range(-9999999.0, 9999999.0);

		//Context->DataManager->createPointCloudObjects(true, Context->Scene.PointCloudObject, SVector3f(-3.f, 0.8f, 3.f), & o);
		//DataParser[0]->createGridDataFromRawValues(FullRange, 5.0, "d1");
		//DataParser[0]->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & sf);
		//DataParser[0]->createVolumeFromGridValues(& sf);
	}

	if (false)
	{
		SciDataParserCTD * Parser = new SciDataParserCTD();
		Parser->Manager = Context->DataManager;
		Parser->examine("oxygenDataSet1.mat");

		//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));

		CSpectrumColorMapper sf("salinity");
		sf.AcceptedRange = Range(-99999.0, 99999.0);

		//DataParser[1]->createPointCloudObjects(true, VoxelObject, SVector3f(3.f), & sf);
		Parser->Manager->createGridDataFromRawValues(sf.AcceptedRange, 5.0, "salinity");
		//DataParser->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & sf);
		//DataParser->createVolumeFromGridValues(& sf);
	}

	if (true)
	{
		SciDataParserGrid1 * Parser = new SciDataParserGrid1();
		Parser->Manager = Context->DataManager;
		Parser->load("oxyMaps.mat");

		//DataParser->GridValues.setDataScale(Vector3(3, 2, 3));
		
		CRGBIntensityColorMapper r("o1", "o2", "o3");
		CSingleFieldColorMapper sf("o1");
		COxygenColorMapper o;
		//COxygenLocalizedColorMapper l;
			
		Context->DataManager->createVolumeFromGridValues(& o);
		/*Context->DataManager->createPointCloudObjects(false, Context->Scene.GridObject, SVector3f(3.f), & o);
		Context->Scene.VolumeSceneObject->VolumeHandle = Context->DataManager->VolumeHandle;*/
	}

	if (true)
	{
		SciDataParserCSV * Parser1 = new SciDataParserCSV();
		Parser1->Manager = Context->DataManager;
		Parser1->mergedLoad("OxyData.csv", "LogData.csv", "Time");
		/*
		COxygenColorMapper o("AirSaturation(%)");

		Context->DataManager->createPointCloudObjects(true, Context->Scene.PointCloudObject, SVector3f(1.f/*-3.f, 0.8f, 3.f*), & o);*/
	}

	Context->DataManager->writeToFile("Datasets/HopavagenBay1.dat");
}
