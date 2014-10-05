
#include "CTerrainLocation.h"
#include "CTerrainSceneObject.h"


void CTerrainLocation::Load()
{
	ColorImage = CImage::Load(ColorFile);
	HeightImage = CImage::Load(HeightFile);
	BathymetryImage = CImage::Load(BathymetryFile);
}

void CTerrainLocation::ConcurrentLoad()
{	
	ColorMap = ColorImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
	HeightMap = HeightImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
	BathymetryMap = BathymetryImage->MakeTexture()->SetWrapMode(CTexture2D::EWrapMode::Clamp);
}

void CTerrainLocation::InitSceneElements(CProgramContext::SScene & Scene)
{
	//Scene.Terrain->SetColorMap(ColorMap);
	//Scene.Terrain->SetHeightMap(HeightMap);
	//Scene.Terrain->SetBathymetryMap(BathymetryMap);
}
