
#pragma once

#include "ILocation.h"
#include "CDataSet.h"


class CSite
{

public:
	
	CSite(string const & Name);

	void ReadConfiguration();

	void Load();
	void ConcurrentLoad();
	void InitSceneElements(CProgramContext::SScene & Scene);

	void AddDataSet(CDataSet * DataSet);
	CDataSet * GetCurrentDataSet();
	ILocation * GetCurrentLocation();

	string const & GetName() const;
	string const & GetPath() const;

protected:
	
	std::vector<ILocation *> Locations;
	std::vector<CDataSet *> DataSets;

	string Name;
	string Path;

	struct SConfiguration
	{
		struct SDataSet
		{
			string PositionXField;
			string PositionYField;
			string PositionZField;

			struct SAsset
			{
				string File;
				string Parser;
			};

			vector<SAsset> Assets;
		};

		vector<SDataSet> DataSets;

		struct SLocation
		{
			struct SBound
			{
				string Longitude;
				string Latitude;
			};

			SBound LowerBound;
			SBound UpperBound;

			string ColorFile;
			string BathymetryFile;
			string HeightFile;
		};

		vector<SLocation> Locations;
	};

	SConfiguration Configuration;

};
