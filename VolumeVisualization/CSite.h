
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

};
