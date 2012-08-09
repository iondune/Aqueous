#include "SciDataParser.h"

void SciDataParser::createDataTreeFromRawValues()
{
	CSciTreeLeaf * Root = (CSciTreeLeaf *) (DataTree = new CSciTreeLeaf());

	for (auto it = RawValues.Values.begin(); it != RawValues.Values.end(); ++ it)
		Root->Datums.push_back(* it);
	Root->Extents = SBoundingBox3f(Vector3(0), RawValues.DataScale);
		
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
	//unsigned int t0 = (unsigned int) time(0), t1;
	SubdivideNode(DataTree);
	//t1 = (unsigned int) time(0);
	//printf("End subdivision. Took %d ms.\n", t1 - t0);

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
