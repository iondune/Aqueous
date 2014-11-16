
#include "CLocation.h"
#include "CTerrainNodeManager.h"
#include "CSite.h"


CLocation::CLocation(CSite * Site)
{
	this->Site = Site;
}

void CLocation::Load()
{
	ColorImage = CImage::Load(Site->GetPath() + ColorFile);
	HeightImage = CImage::Load(Site->GetPath() + HeightFile);
	BathymetryImage = CImage::Load(Site->GetPath() + BathymetryFile);
}

void CLocation::ConcurrentLoad()
{	
	if (ColorImage)
		ColorMap = ColorImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
	if (HeightImage)
		HeightMap = HeightImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
	if (BathymetryImage)
		BathymetryMap = BathymetryImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
}

void CLocation::InitSceneElements(CProgramContext::SScene & Scene)
{
	Scene.Terrain->SetColorMap(ColorMap);
	Scene.Terrain->SetHeightMap(HeightMap);
	Scene.Terrain->SetBathymetryMap(BathymetryMap);
}
