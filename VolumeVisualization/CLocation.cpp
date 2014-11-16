
#include "CLocation.h"
#include "CTerrainNodeManager.h"


void CLocation::Load()
{
	ColorImage = CImage::Load(ColorFile);
	HeightImage = CImage::Load(HeightFile);
	BathymetryImage = CImage::Load(BathymetryFile);
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
