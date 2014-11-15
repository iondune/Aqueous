
#include "CSite.h"

#include "CTerrainLocation.h"

#include <rapidjson/document.h>


CSite::CSite(string const & Name)
{
	this->Name = Name;
	this->Path = "Sites/" + Name;
}

void CSite::ReadConfiguration()
{
	rapidjson::Document d;
	d.Parse(File::ReadAsString(Path + "/Site.json").c_str());
	string Error;

	if (d.HasMember("DataSets"))
	{
		auto & dDataSets = d["DataSets"];
		if (dDataSets.IsArray())
		{
			for (uint i = 0; i < dDataSets.Size(); ++ i)
			{
				auto & dDataSet = dDataSets[i];
				SConfiguration::SDataSet DataSet;
				DataSet.PositionXField = dDataSet["PositionXField"].GetString();
				DataSet.PositionYField = dDataSet["PositionYField"].GetString();
				DataSet.PositionZField = dDataSet["PositionZField"].GetString();

				if (dDataSet.HasMember("Assets"))
				{
					auto & dAssets = dDataSet["Assets"];
					for (uint i = 0; i < dAssets.Size(); ++ i)
					{
						auto & dAsset = dAssets[i];
						SConfiguration::SDataSet::SAsset Asset;
						if (dAsset.HasMember("File"))
						{
							if (dAsset["File"].IsString())
								Asset.File = dAsset["File"].GetString();
							else
								cerr << "Failed to read site configuration file: " << "File member is not a string" << endl;
								
						}
						Asset.Parser = dAsset["Parser"].GetString();

						DataSet.Assets.push_back(Asset);
					}
				}
				else
				{
					cerr << "Failed to read site configuration file: " << "unable to find Assets member" << endl;
				}

				Configuration.DataSets.push_back(DataSet);
			}
		}
	}
	else
	{
		cerr << "Failed to read site configuration file: " << "unable to find DataSets member" << endl;
	}

	if (d.HasMember("Locations"))
	{
		auto & dLocations = d["Locations"];
		if (dLocations.IsArray())
		{
			for (uint i = 0; i < dLocations.Size(); ++ i)
			{
				auto & dLocation = dLocations[i];
				SConfiguration::SLocation Location;
				Location.ColorFile = dLocation["ColorFile"].GetString();
				Location.BathymetryFile = dLocation["BathymetryFile"].GetString();
				Location.HeightFile = dLocation["HeightFile"].GetString();

				auto & dLowerBound = dLocation["LowerBound"];
				Location.LowerBound.Longitude = dLowerBound["Longitude"].GetString();
				Location.LowerBound.Latitude = dLowerBound["Latitude"].GetString();

				auto & dUpperBound = dLocation["UpperBound"];
				Location.UpperBound.Longitude = dUpperBound["Longitude"].GetString();
				Location.UpperBound.Latitude = dUpperBound["Latitude"].GetString();

				Configuration.Locations.push_back(Location);
			}
		}
	}
	else
	{
		cerr << "Failed to read site configuration file: " << "unable to find Locations member" << endl;
	}
}

void CSite::Load()
{
	for (auto Location : Locations)
		Location->Load();
	for (auto DataSet : DataSets)
		DataSet->Load();
}

void CSite::ConcurrentLoad()
{
	for (auto Location : Locations)
		Location->ConcurrentLoad();
	for (auto DataSet : DataSets)
		DataSet->ConcurrentLoad();
}

void CSite::InitSceneElements(CProgramContext::SScene & Scene)
{
	Locations[0]->InitSceneElements(Scene);
	DataSets[0]->InitSceneElements(Scene);
}

void CSite::AddDataSet(CDataSet * DataSet)
{
	DataSets.push_back(DataSet);
}

CDataSet * CSite::GetCurrentDataSet()
{
	return DataSets[0];
}

ILocation * CSite::GetCurrentLocation()
{
	return Locations[0];
}

string const & CSite::GetName() const
{
	return Name;
}

string const & CSite::GetPath() const
{
	return Path;
}
