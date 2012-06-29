#include "CMainState.h"

void CMainState::loadData()
{
	// Determine data source
	std::string Field;

	printf("Loading data...\n");
	switch (2)
	{
	default:
	case 0:
		DataParser = new SciDataParserSimpleTXT();
		DataParser->load("ForZoe.txt");
		printf("Performing data operations...\n");
		DataParser->RawValues.setDataScale(Vector3(3, 2, 3));
		Field = "o2";
		break;

	case 1:
		DataParser = new SciDataParserCTD();
		DataParser->load("data2.mat");
		printf("Performing data operations...\n");
		DataParser->RawValues.setDataScale(Vector3(3, 2, 3));
		Field = "salinity";
		break;

	case 2:
		{
			DataParser = new SciDataParserGrid1();
			DataParser->load("oxyMaps.mat");
			printf("Performing data operations...\n");
			DataParser->GridValues.setDataScale(Vector3(3, 2, 3));
		
			printf("Performing volumetric operations...\n");
			CRGBIntensityColorMapper r("o1", "o2", "o3");
			COxygenColorMapper o;
			
			DataParser->generateVolumeFromGridValues(& o);
			Field = "mult";
		}
		break;
	}

	int PointsLoaded = 0;
	
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
	}

	for (auto it = DataParser->GridValues.Values.begin(); it != DataParser->GridValues.Values.end(); ++ it)
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
	}

	printf("Created %d points.\n\n\n", PointsLoaded);
}
