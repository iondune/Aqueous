
#pragma once

#include "CProgramContext.h"


class CLocation
{

public:

	CLocation(CSite * Site);
	
	longlatd LowerBound, UpperBound;

	std::string HeightFile, ColorFile, BathymetryFile;
	CImage * HeightImage = nullptr, * ColorImage = nullptr, * BathymetryImage = nullptr;
	CTexture * HeightMap = nullptr, * ColorMap = nullptr, * BathymetryMap = nullptr;
	
	virtual void Load();
	virtual void ConcurrentLoad();
	virtual void InitSceneElements(CProgramContext::SScene & Scene);

	CSite * Site;

};
