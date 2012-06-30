#include "SciDataParser.h"

#include <time.h>

void SciDataParser::generateVolumeFromGridValues(IColorMapper * ColorMapper)
{
	/// Generate Volume!
	unsigned int const size = GridDimensions[0] * GridDimensions[1] * GridDimensions[2] * 4;

	if (GridValues.Values.size() != size / 4)
	{
		printf("Unexpected size of grid data.\n");
		return;
	}

	ColorMapper->preProcessValues(GridValues);

	GLubyte * volumeData = new GLubyte[size];

	//int ValueIndex = 0;

	for (int i = 0; i < GridDimensions[2]; ++ i)
	{
		for (int j = 0; j < GridDimensions[1]; ++ j)
		{
			for (int k = 0; k < GridDimensions[0]; ++ k)
			{
				int index = k + (GridDimensions[2] - i - 1) * GridDimensions[0] + j * GridDimensions[2] * GridDimensions[0];

				int ValueIndex = k + j * GridDimensions[0] + i * GridDimensions[1] * GridDimensions[0];

				SColor Color = ColorMapper->getColor(GridValues.Values[ValueIndex]);

				for (int t = 0; t < 4; ++ t)
					volumeData[index * 4 + t] = clamp<unsigned char>((unsigned char) (Color[t] * 255.f), 0, 255);

				//++ ValueIndex;
			}
		}
	}


	glEnable(GL_TEXTURE_3D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (! VolumeHandle)
		glGenTextures(1, & VolumeHandle);
	glBindTexture(GL_TEXTURE_3D, VolumeHandle);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, GridDimensions[0], GridDimensions[2], GridDimensions[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, volumeData);
	
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	delete []volumeData;
}

void SciDataParser::createDataTreeFromRawValues()
{
	CSciTreeLeaf * Root = (CSciTreeLeaf *) (DataTree = new CSciTreeLeaf());

	for (auto it = RawValues.Values.begin(); it != RawValues.Values.end(); ++ it)
		Root->Datums.push_back(* it);
	Root->Extents = SBoundingBox3(Vector3(0), RawValues.DataScale);
		
	/*
	auto CreateLeafObjects = []()
	{
		for (int i = 0; i < EO_COUNT; ++ i)
		{
			if (((CSciTreeLeaf *)NewRoot->Children[i])->Datums.size() > 1)
				SubdivideNode(NewRoot->Children[i]);
			else if (((CSciTreeLeaf *)NewRoot->Children[i])->Datums.size() == 1)
			{
				CMeshSceneObject * Object = new CMeshSceneObject();
				Object->setMesh(Cube);
				Object->setParent(SoupObject);
				Object->setScale(NewRoot->Children[i]->Extents.getExtent());
				Object->setTranslation(NewRoot->Children[i]->Extents.getCenter());
				Object->addUniform("uLightPosition", boost::shared_ptr<IUniform const>(& BindLightPosition));

				double o2_ratio = ((CSciTreeLeaf *)NewRoot->Children[i])->Datums[0].getField(Field);
				CRenderable::SMaterial mat;
				mat.DiffuseColor = SColor(1.f - (float) o2_ratio, (float) o2_ratio, 1.f - (float) o2_ratio);
				Object->setMaterial(mat);
				Object->setShader(ERenderPass::Default, Shader);
				//Object->enableDebugData(EDebugData::Wireframe);
				Object->setCullingEnabled(false);
			}
		}
	}
	*/


	printf("Beginning subdivision.\n");
	unsigned int t0 = (unsigned int) time(0), t1;
	SubdivideNode(DataTree);
	t1 = (unsigned int) time(0);
	printf("End subdivision. Took %d ms.\n", t1 - t0);

	CSciTreeNode * NewRoot = (CSciTreeNode *) DataTree;

#if 0
	for (int i = 0; i < EO_COUNT; ++ i)
	{
		CMeshSceneObject * Object = new CMeshSceneObject();
		Object->setMesh(Cube);
		Object->setParent(VoxelObject);
		Object->setScale(NewRoot->Children[i]->Extents.getExtent() / ScaleFactor);
		Object->setTranslation(NewRoot->Children[i]->Extents.getCenter() / ScaleFactor);
		Object->addUniform("uLightPosition", & BindLightPosition);

		CMaterial mat;
		mat.DiffuseColor = SColor(0.8f, 0.8f, 0.8f);
		Object->setMaterial(mat);
		Object->setShader(ERenderPass::ERP_DEFAULT, Shader);
		Object->enableDebugData(EDebugData::Wireframe);
		Object->setCullingEnabled(false);
	}
#endif

}
