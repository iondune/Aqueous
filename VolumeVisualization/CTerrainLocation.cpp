
#include "CTerrainLocation.h"
#include "CTerrainNodeManager.h"


void CTerrainLocation::Load()
{
	ColorImage = CImage::Load(ColorFile);
	HeightImage = CImage::Load(HeightFile);
	BathymetryImage = CImage::Load(BathymetryFile);
}

void CTerrainLocation::ConcurrentLoad()
{	
	if (ColorImage)
		ColorMap = ColorImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
	if (HeightImage)
		HeightMap = HeightImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
	if (BathymetryImage)
		BathymetryMap = BathymetryImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
}

void CTerrainLocation::InitSceneElements(CProgramContext::SScene & Scene)
{
	Scene.Terrain->SetColorMap(ColorMap);
	Scene.Terrain->SetHeightMap(HeightMap);
	Scene.Terrain->SetBathymetryMap(BathymetryMap);
}
