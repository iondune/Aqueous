
#include "CDataSet.h"

#include "ColorMappers.h"
#include "CGlyphSceneObject.h"


CDataSet::CDataSet()
	: VolumeHandle()
{}

void CDataSet::Load()
{
	std::ifstream PointFile, VolumeFile;
	PointFile.open(PointFileName);
	VolumeFile.open(VolumeFileName);

	if (PointFile.is_open())
		Points.ReadFromFile(PointFile);
	if (VolumeFile.is_open())
		Volume.ReadFromFile(VolumeFile);
}

void CDataSet::ConcurrentLoad()
{
	glGenTextures(1, & VolumeHandle);
	CSpectrumColorMapper ColorMapper("Temperature (c)");
	Volume.MakeOpenGLVolume(VolumeHandle, & ColorMapper);
}

void CDataSet::InitSceneElements(CProgramContext::SScene & Scene)
{
	Scene.Volume->VolumeHandle = VolumeHandle;

	CSpectrumColorMapper ColorMapper("Temperature (c)");
	Scene.Glyphs->LoadGlyphs(this, & ColorMapper);
}