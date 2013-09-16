
#include "CTerrainLocation.h"
#include "CTerrainSceneObject.h"


#undef LoadImage // To Do : WHYYYYYYY
void CTerrainLocation::Load()
{
	ColorImage = CImageLoader::LoadImage(ColorFile);
	HeightImage = CImageLoader::LoadImage(HeightFile);
	BathymetryImage = CImageLoader::LoadImage(BathymetryFile);
}

void CTerrainLocation::ConcurrentLoad()
{
	STextureCreationFlags Flags;
	Flags.Wrap = GL_CLAMP_TO_EDGE;
	
	ColorMap = CImageLoader::LoadTexture(ColorImage, Flags);
	HeightMap = CImageLoader::LoadTexture(HeightImage, Flags);
	BathymetryMap = CImageLoader::LoadTexture(BathymetryImage, Flags);
}

void CTerrainLocation::InitSceneElements(CProgramContext::SScene & Scene)
{
	Scene.Terrain->SetColorMap(ColorMap);
	Scene.Terrain->SetHeightMap(HeightMap);
	Scene.Terrain->SetBathymetryMap(BathymetryMap);
}
