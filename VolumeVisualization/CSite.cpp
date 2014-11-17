
#include "CSite.h"

#include "CLocation.h"

#include <rapidjson/document.h>


CSite::CSite(string const & Name)
{
	this->Name = Name;
	this->Path = "Sites/" + Name + "/";
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
				CDataSet * DataSet = new CDataSet(this);
				DataSet->Traits.PositionXField = dDataSet["PositionXField"].GetString();
				DataSet->Traits.PositionYField = dDataSet["PositionYField"].GetString();
				DataSet->Traits.PositionZField = dDataSet["PositionZField"].GetString();
				
				if (dDataSet.HasMember("MapDepth") && dDataSet["MapDepth"].IsDouble())
					DataSet->MapDepth = dDataSet["MapDepth"].GetDouble();

				if (dDataSet.HasMember("DataLonLatCenter") && dDataSet["DataLonLatCenter"].IsObject())
				{
					SLongitudeLatituded DataLonLatCenter;
					DataLonLatCenter.Longitude = SLongitudeLatituded::DMStoDecimal(dDataSet["DataLonLatCenter"]["Longitude"].GetString());
					DataLonLatCenter.Latitude = SLongitudeLatituded::DMStoDecimal(dDataSet["DataLonLatCenter"]["Latitude"].GetString());
					DataSet->DataLonLatCenter = DataLonLatCenter;
					DataSet->ManuallySetDataLongLat = true;
				}

				if (dDataSet.HasMember("Assets"))
				{
					auto & dAssets = dDataSet["Assets"];
					for (uint i = 0; i < dAssets.Size(); ++ i)
					{
						auto & dAsset = dAssets[i];
						CDataSet::SAsset Asset;

						if (dAsset.HasMember("File") && dAsset["File"].IsString())
							Asset.File = dAsset["File"].GetString();
						else
							cerr << "Failed to read site configuration file: " << "File member is not a string or does not exist" << endl;

						if (dAsset.HasMember("Parser") && dAsset["Parser"].IsString())
							Asset.Parser = dAsset["Parser"].GetString();
						else
							cerr << "Failed to read site configuration file: " << "Parser member is not a string or does not exist" << endl;
						
						if (dAsset.HasMember("FieldDelimiter") && dAsset["FieldDelimiter"].IsString())
							Asset.FieldDelimiter = dAsset["FieldDelimiter"].GetString()[0];

						if (dAsset.HasMember("ValueDelimiter") && dAsset["ValueDelimiter"].IsString())
							Asset.ValueDelimiter = dAsset["ValueDelimiter"].GetString()[0];

						DataSet->Assets.push_back(Asset);
					}
				}
				else
				{
					cerr << "Failed to read site configuration file: " << "unable to find Assets member" << endl;
				}

				DataSets.push_back(DataSet);
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
				CLocation * Location = new CLocation(this);
				Location->ColorFile = dLocation["ColorFile"].GetString();
				Location->BathymetryFile = dLocation["BathymetryFile"].GetString();
				Location->HeightFile = dLocation["HeightFile"].GetString();

				auto & dLowerBound = dLocation["LowerBound"];
				Location->LowerBound.Longitude = SLongitudeLatituded::DMStoDecimal(dLowerBound["Longitude"].GetString());
				Location->LowerBound.Latitude = SLongitudeLatituded::DMStoDecimal(dLowerBound["Latitude"].GetString());

				auto & dUpperBound = dLocation["UpperBound"];
				Location->UpperBound.Longitude = SLongitudeLatituded::DMStoDecimal(dUpperBound["Longitude"].GetString());
				Location->UpperBound.Latitude = SLongitudeLatituded::DMStoDecimal(dUpperBound["Latitude"].GetString());

				Locations.push_back(Location);
			}
		}
	}
	else
	{
		cerr << "Failed to read site configuration file: " << "unable to find Locations member" << endl;
	}
}

void CSite::Load(IProgressBar * Progress)
{
	Progress->BeginProgress();

	f64 Total = Locations.size() + DataSets.size();
	for (auto Location : Locations)
		Location->Load(Progress->NewTask(1 / Total));
	for (auto DataSet : DataSets)
		DataSet->Load(Progress->NewTask(1 / Total));

	Progress->EndProgress();
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

CLocation * CSite::GetCurrentLocation()
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

std::vector<CLocation *> const & CSite::GetLocations() const
{
	return Locations;
}

std::vector<CDataSet *> const & CSite::GetDataSets() const
{
	return DataSets;
}
