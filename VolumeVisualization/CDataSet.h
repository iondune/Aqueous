
#pragma once

#include "CProgramContext.h"


class CDataSet
{

public:

	CDataSet();

	void Load();
	void ConcurrentLoad();
	void InitSceneElements(CProgramContext::SScene & Scene);

	struct STraits
	{
		std::string PositionXField, PositionYField, PositionZField;
		bool InvertY;

		STraits()
			: PositionXField("x"), PositionYField("y"), PositionZField("z"), InvertY(true)
		{}
	};

	STraits Traits;

	STable Points;
	SVolumeDatabase<f64> Volume;
	u32 VolumeHandle;

	longlatd LowerBounds, UpperBounds;
	std::string PointFileName, VolumeFileName;
	std::string SourceFile;

};
