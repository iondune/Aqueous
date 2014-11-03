
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
	CSpectrumColorMapper ColorMapper("o2");
	Volume.MakeOpenGLVolume(VolumeHandle, & ColorMapper);



	vec3u Dimensions = Volume.Dimensions; // (10);
	u8 * const VolumeData = new u8[Dimensions.X * Dimensions.Y * Dimensions.Z];

	SRange<f64> XRange = Points.GetFieldRange(Traits.PositionXField, 15.0);
	SRange<f64> YRange = Points.GetFieldRange(Traits.PositionYField, 15.0);
	SRange<f64> ZRange = Points.GetFieldRange(Traits.PositionZField, 15.0);

	if (XRange.IsEmpty())
		XRange = SRange<f64>(-1, 1);
	if (YRange.IsEmpty())
		YRange = SRange<f64>(-1, 1);
	if (ZRange.IsEmpty())
		ZRange = SRange<f64>(-1, 1);

	for (u32 k = 0; k < Dimensions.Z; ++ k)
		for (u32 j = 0; j < Dimensions.Y; ++ j)
			for (u32 i = 0; i < Dimensions.X; ++ i)
			{
				u32 const Index = i + j * Dimensions.X + k * Dimensions.X * Dimensions.Y;

				f32 MinDistance = 1.0;
				for (auto Point : Points.GetValues())
				{
					f32 X = (f32) XRange.Normalize(Point.GetField(Traits.PositionXField));
					if (XRange.IsEmpty())
						X = 0.f;

					f32 Y = (f32) YRange.Normalize(Point.GetField(Traits.PositionYField));
					if (Traits.InvertY)
						Y = 1.f - Y;
					if (YRange.IsEmpty())
						Y = 0.f;

					f32 Z = (f32) ZRange.Normalize(Point.GetField(Traits.PositionZField));
					if (ZRange.IsEmpty())
						Z = 0.f;

					f32 Distance = vec3f(X, Y, Z).GetDistanceSqFrom(vec3f(i / (f32) Dimensions.X, j / (f32) Dimensions.Y, k / (f32) Dimensions.Z));
					if (Distance < MinDistance)
						MinDistance = Distance;
				}

				f64 Value = 1.0 - 24.0 * MinDistance;
				if (Value < 0)
					Value = 0;
				VolumeData[Index] = Clamp<u32>(Value * 255.0, 0, 255);
			}

	glGenTextures(1, & ProximityTextureHandle);
	glBindTexture(GL_TEXTURE_3D, ProximityTextureHandle);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, Dimensions.X, Dimensions.Y, Dimensions.Z, 0, GL_RED, GL_UNSIGNED_BYTE, VolumeData);
	glBindTexture(GL_TEXTURE_3D, 0);
}

void CDataSet::InitSceneElements(CProgramContext::SScene & Scene)
{
	Scene.Volume->VolumeData = new CTexture3D(VolumeHandle);
	Scene.Volume->ProximityTexture = new CTexture3D(ProximityTextureHandle);

	CSpectrumColorMapper ColorMapper("o2");
	//ColorMapper.ValueCutoff = 1.0;
	//Scene.Glyphs->LoadGlyphs(this, & ColorMapper);
}
