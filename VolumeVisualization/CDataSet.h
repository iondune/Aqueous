
#pragma once

#include "CProgramContext.h"


class CDataSet
{

public:

	CDataSet(CSite * Site);

	void Load(IProgressBar::CTask * Task);
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

	struct SAsset
	{
		string File;
		string Parser;
		char FieldDelimiter = ',';
		char ValueDelimiter = ',';
	};

	vector<SAsset> Assets;

	STable Points;
	SVolumeDatabase<f64> Volume;
	u32 VolumeHandle;
	u32 ProximityTextureHandle;

	longlatd LowerBounds, UpperBounds;
	std::string PointFileName, VolumeFileName;
	std::string SourceFile;

	bool ManuallySetDataLongLat = false;
	longlatd DataLonLatCenter;

	f64 YExaggeration = 8.0;
	f64 MapDepth = 180.0;

	CSite * Site;

};
