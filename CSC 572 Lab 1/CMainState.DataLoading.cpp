#include "CMainState.h"

#include "ColorMappers.h"

void CMainState::loadData()
{
	// Determine data source
	//std::string Field;

	printf("Loading data...\n");
	{
		DataParser[0] = new SciDataParserSimpleTXT();
		DataParser[0]->load("ForZoe.txt");

		//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));

		CSpectrumColorMapper sf("d1");
		COxygenColorMapper o("d1");
		sf.AcceptedRange = Range(-9999999.0, 9999999.0);

		DataParser[0]->createPointCloudObjects(true, VoxelObject, SVector3f(3.f), & o);
		//DataParser[0]->createGridDataFromRawValues(FullRange, 5.0, "d1");
		//DataParser[0]->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & sf);
		//DataParser[0]->createVolumeFromGridValues(& sf);
	}

	{
		DataParser[1] = new SciDataParserCTD();
		//DataParser[1]->load("data2.mat");

		//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));

		CSpectrumColorMapper sf("salinity");
		sf.AcceptedRange = Range(-99999.0, 99999.0);

		//DataParser[1]->createPointCloudObjects(true, VoxelObject, SVector3f(3.f), & sf);
		//DataParser->createGridDataFromRawValues(sf.AcceptedRange, 5.0, "salinity");
		//DataParser->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & sf);
		//DataParser->createVolumeFromGridValues(& sf);
	}

	{
		DataParser[2] = new SciDataParserGrid1();
		DataParser[2]->load("oxyMaps.mat");

		//DataParser->GridValues.setDataScale(Vector3(3, 2, 3));
		
		CRGBIntensityColorMapper r("o1", "o2", "o3");
		CSingleFieldColorMapper sf("o1");
		COxygenColorMapper o;
		//COxygenLocalizedColorMapper l;
			
		DataParser[2]->createVolumeFromGridValues(& o);
		DataParser[2]->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & o);
	}
}
