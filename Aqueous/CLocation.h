
#pragma once

#include "CProgramContext.h"


class CLocation
{

public:

	CLocation(CSite * Site);
	
	longlatd LowerBound, UpperBound;

	std::string HeightFile, ColorFile, BathymetryFile, NormalFile;
	CImage * HeightImage = nullptr, * ColorImage = nullptr, * BathymetryImage = nullptr, * NormalImage = nullptr;
	CTexture * HeightMap = nullptr, * ColorMap = nullptr, * BathymetryMap = nullptr, * NormalMap = nullptr;
	
	virtual void Load(IProgressBar::CTask * Task);
	virtual void ConcurrentLoad();
	virtual void InitSceneElements(CProgramContext::SScene & Scene);

	CSite * Site;

};
