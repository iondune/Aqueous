#ifndef _SCI_DATA_TREE_H_INCLUDED_
#define _SCI_DATA_TREE_H_INCLUDED_

#include "ionCore.h"
#include <vector>

#include "SciData.h"


class ISciTreeNode
{

public:

	SBoundingBox3f Extents;

	virtual bool const isLeaf() const =0;

	virtual ~ISciTreeNode()
	{}

};

enum EOctant
{
	EO_TOP_UPPER_LEFT,
	EO_TOP_UPPER_RIGHT,
	EO_TOP_LOWER_LEFT,
	EO_TOP_LOWER_RIGHT,
	EO_BOTTOM_UPPER_LEFT,
	EO_BOTTOM_UPPER_RIGHT,
	EO_BOTTOM_LOWER_LEFT,
	EO_BOTTOM_LOWER_RIGHT,
	EO_COUNT
};

class CSciTreeNode : public ISciTreeNode
{

public:

	ISciTreeNode * Children[EO_COUNT];

	virtual bool const isLeaf() const
	{
		return false;
	}

};

class CSciTreeLeaf : public ISciTreeNode
{

public:

	std::vector<SciData> Datums;

	virtual bool const isLeaf() const
	{
		return true;
	}

};

void SubdivideNode(ISciTreeNode * & Node);

#endif
