#include "SciDataTree.h"

#include <ionScene.h>

void SubdivideNode(ISciTreeNode * & Node)
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
			NewRoot->Children[i]->Extents = SBoundingBox3f(
				SVector3f(Root->Extents.MinCorner.X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z),
				SVector3f(Root->Extents.getCenter().X, Root->Extents.MaxCorner.Y, Root->Extents.MaxCorner.Z)
				);
			break;

		case EO_TOP_UPPER_RIGHT:
			NewRoot->Children[i]->Extents = SBoundingBox3f(
				SVector3f(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z),
				SVector3f(Root->Extents.MaxCorner.X, Root->Extents.MaxCorner.Y, Root->Extents.MaxCorner.Z)
				);
			break;

		case EO_TOP_LOWER_LEFT:
			NewRoot->Children[i]->Extents = SBoundingBox3f(
				SVector3f(Root->Extents.MinCorner.X, Root->Extents.MinCorner.Y, Root->Extents.getCenter().Z),
				SVector3f(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.MaxCorner.Z)
				);
			break;

		case EO_TOP_LOWER_RIGHT:
			NewRoot->Children[i]->Extents = SBoundingBox3f(
				SVector3f(Root->Extents.getCenter().X, Root->Extents.MinCorner.Y, Root->Extents.getCenter().Z),
				SVector3f(Root->Extents.MaxCorner.X, Root->Extents.getCenter().Y, Root->Extents.MaxCorner.Z)
				);
			break;

		case EO_BOTTOM_UPPER_LEFT:
			NewRoot->Children[i]->Extents = SBoundingBox3f(
				SVector3f(Root->Extents.MinCorner.X, Root->Extents.getCenter().Y, Root->Extents.MinCorner.Z),
				SVector3f(Root->Extents.getCenter().X, Root->Extents.MaxCorner.Y, Root->Extents.getCenter().Z)
				);
			break;

		case EO_BOTTOM_UPPER_RIGHT:
			NewRoot->Children[i]->Extents = SBoundingBox3f(
				SVector3f(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.MinCorner.Z),
				SVector3f(Root->Extents.MaxCorner.X, Root->Extents.MaxCorner.Y, Root->Extents.getCenter().Z)
				);
			break;

		case EO_BOTTOM_LOWER_LEFT:
			NewRoot->Children[i]->Extents = SBoundingBox3f(
				SVector3f(Root->Extents.MinCorner.X, Root->Extents.MinCorner.Y, Root->Extents.MinCorner.Z),
				SVector3f(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z)
				);
			break;

		case EO_BOTTOM_LOWER_RIGHT:
			NewRoot->Children[i]->Extents = SBoundingBox3f(
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
};
