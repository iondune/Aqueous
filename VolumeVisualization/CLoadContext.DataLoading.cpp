#include "CLoadContext.h"

#include "CMainState.h"
#include "ColorMappers.h"


void CLoadContext::loadData()
{
	Indent = 60;

	Context->DataManager = new SciDataManager();

	if (true)
	{
		addLabel(L"Loading Binary Data...");
		Context->DataManager->readFromFile("HopavagenBayDataSet.dat");
		{		
			addLabel(L"Calculating...");

			COxygenColorMapper o("d1");
			Context->DataManager->createPointCloudObjects(true, Context->Scene.PointCloudObject, SVector3f(-3.f, 0.8f, 3.f), & o);

			o.Field = "o1";
			Context->DataManager->createVolumeFromGridValues(& o);
			Context->DataManager->createPointCloudObjects(false, Context->Scene.GridObject, SVector3f(3.f), & o);
			Context->Scene.VolumeSceneObject->VolumeHandle = Context->DataManager->VolumeHandle;

			std::cout << "Succesfully created visual objects from data." << std::endl;
		}
	}
	else
	{
		{
			addLabel(L"Data Set 1...");
			SciDataParser * Parser = new SciDataParserSimpleTXT();
			Parser->Manager = Context->DataManager;
			Parser->load("ForZoe.txt");

			//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));
		
			addLabel(L"Calculating...");
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
			addLabel(L"Data Set 2...");
			SciDataParser * Parser = new SciDataParserGrid1();
			Parser->Manager = Context->DataManager;
			Parser->load("oxyMaps.mat");

			//DataParser->GridValues.setDataScale(Vector3(3, 2, 3));
		
			addLabel(L"Calculating...");
			CRGBIntensityColorMapper r("o1", "o2", "o3");
			CSingleFieldColorMapper sf("o1");
			COxygenColorMapper o;
			//COxygenLocalizedColorMapper l;
			
			Context->DataManager->createVolumeFromGridValues(& o);
			Context->DataManager->createPointCloudObjects(false, Context->Scene.GridObject, SVector3f(3.f), & o);
			Context->Scene.VolumeSceneObject->VolumeHandle = Context->DataManager->VolumeHandle;
		}
	
		addLabel(L"Writing Files...");
		Context->DataManager->writeToFile("HopavagenBayDataSet.dat");
	}

	Indent = 0;
}
