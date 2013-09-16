
#pragma once

#include "ILocation.h"
#include "CDataSet.h"


class CSite
{

public:
	
	CSite();

	void Load();
	void ConcurrentLoad();
	void InitSceneElements(CProgramContext::SScene & Scene);

	void AddDataSet(CDataSet * DataSet);
	CDataSet * GetCurrentDataSet();
	ILocation * GetCurrentLocation();

protected:
	
	std::vector<ILocation *> Locations;
	std::vector<CDataSet *> DataSets;

};
