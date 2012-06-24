#include "CMainState.h"

void CMainState::loadData()
{
	// Determine data source
	std::string Field;
	switch (1)
	{
	default:
	case 0:
		DataSet.parseTXTFile("ForZoe.txt");
		Field = "o2";
		break;
	case 1:
		DataSet.parseMATFile("data2.mat");
		Field = "salinity";
		break;
	}
	DataSet.setDataScale(Vector3(3, 2, 3));

	int PointsLoaded = 0;

	for (auto it = DataSet.Values.begin(); it != DataSet.Values.end(); ++ it)
	{
		CMeshSceneObject * Object = new CMeshSceneObject();
		Object->setMesh(Cube);
		Object->setParent(VoxelObject);
		Object->setScale(SVector3f(1.f) / 32.f);
		Object->setTranslation(SVector3f((float) it->getLocation().X, (float) it->getLocation().Y, (float) it->getLocation().Z));
		Object->addUniform("uLightPosition", boost::shared_ptr<IUniform const>(& BindLightPosition));

		double o2_ratio = it->getField(Field);
		CRenderable::SMaterial mat;
		mat.DiffuseColor = SColor(1.f - (float) o2_ratio, (float) o2_ratio, 1.f - (float) o2_ratio);
		Object->setMaterial(mat);
		Object->setShader(ERenderPass::Default, Shader);
		Object->setCullingEnabled(false);

		PointsLoaded ++;
	}

	printf("Created %d points\n\n\n.", PointsLoaded);

	CSciTreeLeaf * Root = (CSciTreeLeaf *) (DataTree = new CSciTreeLeaf());

	for (auto it = DataSet.Values.begin(); it != DataSet.Values.end(); ++ it)
		Root->Datums.push_back(* it);
	Root->Extents = SBoundingBox3(Vector3(0), DataSet.DataScale);

	std::function<void(ISciTreeNode * & Node)> SubdivideNode;
	SubdivideNode = [&](ISciTreeNode * & Node)
	{
		if (! Node->isLeaf())
			return;

		CSciTreeLeaf * Root = (CSciTreeLeaf *) Node;

		CSciTreeNode * NewRoot = new CSciTreeNode();
		NewRoot->Extents = Root->Extents;

		for (int i = 0; i < EO_COUNT; ++ i)
		{
			NewRoot->Children[i] = new CSciTreeLeaf();

			switch (i)
			{
			case EO_TOP_UPPER_LEFT:
				NewRoot->Children[i]->Extents = SBoundingBox3(
					SVector3f(Root->Extents.MinCorner.X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z),
					SVector3f(Root->Extents.getCenter().X, Root->Extents.MaxCorner.Y, Root->Extents.MaxCorner.Z)
					);
				break;

			case EO_TOP_UPPER_RIGHT:
				NewRoot->Children[i]->Extents = SBoundingBox3(
					SVector3f(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z),
					SVector3f(Root->Extents.MaxCorner.X, Root->Extents.MaxCorner.Y, Root->Extents.MaxCorner.Z)
					);
				break;

			case EO_TOP_LOWER_LEFT:
				NewRoot->Children[i]->Extents = SBoundingBox3(
					SVector3f(Root->Extents.MinCorner.X, Root->Extents.MinCorner.Y, Root->Extents.getCenter().Z),
					SVector3f(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.MaxCorner.Z)
					);
				break;

			case EO_TOP_LOWER_RIGHT:
				NewRoot->Children[i]->Extents = SBoundingBox3(
					SVector3f(Root->Extents.getCenter().X, Root->Extents.MinCorner.Y, Root->Extents.getCenter().Z),
					SVector3f(Root->Extents.MaxCorner.X, Root->Extents.getCenter().Y, Root->Extents.MaxCorner.Z)
					);
				break;

			case EO_BOTTOM_UPPER_LEFT:
				NewRoot->Children[i]->Extents = SBoundingBox3(
					SVector3f(Root->Extents.MinCorner.X, Root->Extents.getCenter().Y, Root->Extents.MinCorner.Z),
					SVector3f(Root->Extents.getCenter().X, Root->Extents.MaxCorner.Y, Root->Extents.getCenter().Z)
					);
				break;

			case EO_BOTTOM_UPPER_RIGHT:
				NewRoot->Children[i]->Extents = SBoundingBox3(
					SVector3f(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.MinCorner.Z),
					SVector3f(Root->Extents.MaxCorner.X, Root->Extents.MaxCorner.Y, Root->Extents.getCenter().Z)
					);
				break;

			case EO_BOTTOM_LOWER_LEFT:
				NewRoot->Children[i]->Extents = SBoundingBox3(
					SVector3f(Root->Extents.MinCorner.X, Root->Extents.MinCorner.Y, Root->Extents.MinCorner.Z),
					SVector3f(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z)
					);
				break;

			case EO_BOTTOM_LOWER_RIGHT:
				NewRoot->Children[i]->Extents = SBoundingBox3(
					SVector3f(Root->Extents.getCenter().X, Root->Extents.MinCorner.Y, Root->Extents.MinCorner.Z),
					SVector3f(Root->Extents.MaxCorner.X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z)
					);
				break;
			}
		}

		for (auto it = Root->Datums.begin(); it != Root->Datums.end(); ++ it)
		{
			for (int i = 0; i < EO_COUNT; ++ i)
			{
				if (NewRoot->Children[i]->Extents.isPointInside(it->getLocation()))
					((CSciTreeLeaf *)NewRoot->Children[i])->Datums.push_back(* it);
			}
		}

		delete Root;
		Node = NewRoot;

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
	};

	printf("Beginning subdivision.\n");
	unsigned int t0 = SDL_GetTicks(), t1;
	SubdivideNode(DataTree);
	t1 = SDL_GetTicks();
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
