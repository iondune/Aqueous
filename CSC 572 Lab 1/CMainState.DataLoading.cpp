#include "CMainState.h"

#include "ColorMappers.h"

void CMainState::loadData()
{
	// Determine data source
	//std::string Field;

	printf("Loading data...\n");
	switch (0)
	{
	default:
	case 0:
		{
			DataParser = new SciDataParserSimpleTXT();
			DataParser->load("ForZoe.txt");

			//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));

			CSingleFieldColorMapper sf("o2");
			sf.AcceptedRange = Range(-9999999.0, 9999999.0);

			DataParser->createPointCloudObjects(true, VoxelObject, SVector3f(3.f), & sf);
			DataParser->createGridDataFromRawValues();
			DataParser->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & sf);
			DataParser->createVolumeFromGridValues(& sf);
		}
		break;

	case 1:
		{
			DataParser = new SciDataParserCTD();
			DataParser->load("data2.mat");

			//DataParser->RawValues.setDataScale(Vector3(3, 2, 3));

			CSingleFieldColorMapper sf("salinity");

			DataParser->createPointCloudObjects(true, VoxelObject, SVector3f(3.f), & sf);
		}
		break;

	case 2:
		{
			DataParser = new SciDataParserGrid1();
			DataParser->load("oxyMaps.mat");

			//DataParser->GridValues.setDataScale(Vector3(3, 2, 3));
		
			CRGBIntensityColorMapper r("o1", "o2", "o3");
			CSingleFieldColorMapper sf("o1");
			COxygenColorMapper o;
			COxygenLocalizedColorMapper l;
			
			DataParser->createVolumeFromGridValues(& o);
			DataParser->createPointCloudObjects(false, SoupObject, SVector3f(3.f), & o);
		}
		break;

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
