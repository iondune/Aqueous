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

	/*int PointsLoaded = 0;
	
	printf("Creating visualization...\n");
	for (auto it = DataParser->RawValues.Values.begin(); it != DataParser->RawValues.Values.end(); ++ it)
	{
		CMeshSceneObject * Object = new CMeshSceneObject();
		Object->setMesh(Cube);
		Object->setParent(VoxelObject);
		Object->setScale(SVector3f(1.f) / 32.f);
		Object->setTranslation(SVector3f((float) it->getLocation().X, (float) it->getLocation().Y, (float) it->getLocation().Z));
		Object->addUniform("uLightPosition", boost::shared_ptr<IUniform const>(& BindLightPosition));

		CRenderable::SMaterial mat;
		if (Field == "mult") 
		{
			mat.DiffuseColor = SColor((float) it->getField("o1"), (float) it->getField("o2"), (float) it->getField("o3"));
		}
		else
		{
			double o2_ratio = it->getField(Field);
			mat.DiffuseColor = SColor(1.f - (float) o2_ratio, (float) o2_ratio, 1.f - (float) o2_ratio);
		}
		Object->setMaterial(mat);
		Object->setShader(ERenderPass::Default, Shader);
		Object->setCullingEnabled(false);

		PointsLoaded ++;
	}*/
}
