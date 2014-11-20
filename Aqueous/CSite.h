
#pragma once

#include "CLocation.h"
#include "CDataSet.h"


class CSite
{

public:
	
	CSite(string const & Name);

	void ReadConfiguration();

	void Load(IProgressBar * Progress);
	void ConcurrentLoad();
	void InitSceneElements(CProgramContext::SScene & Scene);

	void AddDataSet(CDataSet * DataSet);
	CDataSet * GetCurrentDataSet();
	CLocation * GetCurrentLocation();

	string const & GetName() const;
	string const & GetPath() const;

	std::vector<CLocation *> const & GetLocations() const;
	std::vector<CDataSet *> const & GetDataSets() const;

	void SetSelectedLocation(int const SelectedLocation);

protected:
	
	std::vector<CLocation *> Locations;
	std::vector<CDataSet *> DataSets;

	int SelectedLocation = 0;
	int SelectedDataSet = 0;

	string Name;
	string Path;

};
