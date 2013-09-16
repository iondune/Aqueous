
#pragma once

#include "ILocation.h"


class CTerrainLocation : public ILocation
{

public:

	std::string HeightFile, ColorFile, BathymetryFile;
	CImage * HeightImage, * ColorImage, * BathymetryImage;
	CTexture * HeightMap, * ColorMap, * BathymetryMap;

	void Load();
	void ConcurrentLoad();
	void InitSceneElements(CProgramContext::SScene & Scene);

};
