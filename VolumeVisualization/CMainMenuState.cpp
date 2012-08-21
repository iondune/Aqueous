#include "CMainMenuState.h"

#include "CMainState.h"
#include "ColorMappers.h"


CMainMenuState::CMainMenuState()
{}

void CMainMenuState::begin()
{
	Context->GUIContext->setupMenuState();
}

void CMainMenuState::end()
{
	Context->GUIContext->clear();
}

void CMainMenuState::OnRenderStart(float const Elapsed)
{
	Context->GUIContext->draw(Elapsed, true);
	CApplication::get().swapBuffers();
}

void CMainMenuState::OnWindowResized(SWindowResizedEvent const & Event)
{
	Context->GUIContext->getCanvas()->SetSize(Event.Size.X, Event.Size.Y);
	Context->GUIContext->getCanvas()->Invalidate();
	Context->GUIContext->getCanvas()->InvalidateChildren(true);
}

void CMainMenuState::loadData(std::string const & FileName)
{
	std::stringstream s;
	s << "Datasets/";
	s << FileName;
	Context->DataManager->readFromFile(s.str());

	COxygenColorMapper o("d1");
	Context->DataManager->createPointCloudObjects(true, Context->Scene.PointCloudObject, SVector3f(-3.f, 0.8f, 3.f), & o);

	o.Field = "o1";
	Context->DataManager->createVolumeFromGridValues(& o);
	Context->DataManager->createPointCloudObjects(false, Context->Scene.GridObject, SVector3f(3.f), & o);
	Context->Scene.VolumeSceneObject->VolumeHandle = Context->DataManager->VolumeHandle;

	CApplication::get().getStateManager().setState(& CMainState::get());
}

void CMainMenuState::createDataSet()
{
	{
		SciDataParser * Parser = new SciDataParserSimpleTXT();
		Parser->Manager = Context->DataManager;
		Parser->load("ForZoe.txt");

		//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));
		
		CSpectrumColorMapper sf("d1");
		COxygenColorMapper o("d1");
		sf.AcceptedRange = Range(-9999999.0, 9999999.0);

		Context->DataManager->createPointCloudObjects(true, Context->Scene.PointCloudObject, SVector3f(-3.f, 0.8f, 3.f), & o);
		//DataParser[0]->createGridDataFromRawValues(FullRange, 5.0, "d1");
		//DataParser[0]->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & sf);
		//DataParser[0]->createVolumeFromGridValues(& sf);
	}

	{
		//addLabel(L"Data Set 2...");
		//Context->DataParser[1] = new SciDataParserCTD();
		//DataParser[1]->load("data2.mat");

		//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));

		//CSpectrumColorMapper sf("salinity");
		//sf.AcceptedRange = Range(-99999.0, 99999.0);

		//DataParser[1]->createPointCloudObjects(true, VoxelObject, SVector3f(3.f), & sf);
		//DataParser->createGridDataFromRawValues(sf.AcceptedRange, 5.0, "salinity");
		//DataParser->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & sf);
		//DataParser->createVolumeFromGridValues(& sf);
	}

	{
		SciDataParser * Parser = new SciDataParserGrid1();
		Parser->Manager = Context->DataManager;
		Parser->load("oxyMaps.mat");

		//DataParser->GridValues.setDataScale(Vector3(3, 2, 3));
		
		CRGBIntensityColorMapper r("o1", "o2", "o3");
		CSingleFieldColorMapper sf("o1");
		COxygenColorMapper o;
		//COxygenLocalizedColorMapper l;
			
		Context->DataManager->createVolumeFromGridValues(& o);
		Context->DataManager->createPointCloudObjects(false, Context->Scene.GridObject, SVector3f(3.f), & o);
		Context->Scene.VolumeSceneObject->VolumeHandle = Context->DataManager->VolumeHandle;
	}
	
	Context->DataManager->writeToFile("Datasets/HopavagenBayDataSet [2].dat");
}
