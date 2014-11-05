
#pragma once

#include "ILocation.h"


class CTerrainLocation : public ILocation
{

public:

	std::string HeightFile, ColorFile, BathymetryFile;
	CImage * HeightImage = nullptr, * ColorImage = nullptr, * BathymetryImage = nullptr;
	CTexture * HeightMap = nullptr, * ColorMap = nullptr, * BathymetryMap = nullptr;

	void Load();
	void ConcurrentLoad();
	void InitSceneElements(CProgramContext::SScene & Scene);

};
