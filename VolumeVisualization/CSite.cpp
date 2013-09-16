
#include "CSite.h"

#include "CTerrainLocation.h"


CSite::CSite()
{
	CTerrainLocation * Location = new CTerrainLocation;
	Location->LowerBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 55, 45.32f);
	Location->LowerBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 38, 17.22f);
	Location->UpperBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 2, 34.80f);
	Location->UpperBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 41, 59.01f);
	Location->ColorFile = "../Sites/Denmark/TerrainColor.bmp";
	Location->BathymetryFile = "../Sites/Denmark/TerrainBathymetry.bmp";
	Location->HeightFile = "../Sites/Denmark/TerrainTopography.bmp";
	Locations.push_back(Location);

	Location = new CTerrainLocation;
	Location->LowerBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 49, 27.68f);
	Location->LowerBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 34, 20.96f);
	Location->UpperBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 11, 1.75f);
	Location->UpperBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 46, 11.45f);
	Location->ColorFile = "../Sites/Denmark/TerrainColorLarge.bmp";
	Location->BathymetryFile = "../Sites/Denmark/TerrainBathymetryLarge.bmp";
	Location->HeightFile = "../Sites/Denmark/TerrainTopography.bmp";
	Locations.push_back(Location);
	
	Location = new CTerrainLocation;
	Location->LowerBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 54, 13.29f);
	Location->LowerBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 30, 37.33f);
	Location->UpperBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 27, 10.16f);
	Location->UpperBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 48, 48.57f);
	Location->ColorFile = "../Sites/Denmark/TerrainColorXLarge.bmp";
	Location->BathymetryFile = "../Sites/Denmark/TerrainBathymetryXLarge.bmp";
	Location->HeightFile = "../Sites/Denmark/TerrainTopography.bmp";
	Locations.push_back(Location);
	
	Location = new CTerrainLocation;
	Location->LowerBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 39, 1.38f);
	Location->LowerBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 37, 13.75f);
	Location->UpperBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 17, 17.79f);
	Location->UpperBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 58, 5.72f);
	Location->ColorFile = "../Sites/Denmark/Map6Color.bmp";
	Location->BathymetryFile = "../Sites/Denmark/Map6Bathy.bmp";
	Location->HeightFile = "../Sites/Denmark/TerrainTopography.bmp";
	Locations.push_back(Location);
	
	Location = new CTerrainLocation;
	Location->LowerBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 4, 33.9f);
	Location->LowerBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 13, 56.68f);
	Location->UpperBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 53, 14.45f);
	Location->UpperBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(57, 13, 29.39f);
	Location->ColorFile = "../Sites/Denmark/Map7Color.bmp";
	Location->BathymetryFile = "../Sites/Denmark/Map7Bathy.bmp";
	Location->HeightFile = "../Sites/Denmark/TerrainTopography.bmp";
	Locations.push_back(Location);
	
	Location = new CTerrainLocation;
	Location->LowerBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 58, 24.15f);
	Location->LowerBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 39, 43.63f);
	Location->UpperBound.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 0, 17.12f);
	Location->UpperBound.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 40, 45.07f);
	Location->ColorFile = "../Sites/Denmark/Map8.bmp";
	Location->BathymetryFile = "Grey7";
	Location->HeightFile = "../Sites/Denmark/TerrainTopography.bmp";
	Locations.push_back(Location);


	CDataSet * DataSet = new CDataSet;
	DataSets.push_back(DataSet);
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
