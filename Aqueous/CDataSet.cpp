
#include "CDataSet.h"

#include "ColorMappers.h"
#include "CGlyphNodeManager.h"
#include "SciDataParser.h"
#include "CSite.h"
#include "RBFInterpolator/RBFInterpolator.h"


CDataSet::CDataSet(CSite * Site)
	: VolumeHandle()
{
	this->Site = Site;
}

void CDataSet::Load(IProgressBar::CTask * Task)
{
	std::ifstream PointFile, VolumeFile;
	PointFile.open(PointFileName);
	VolumeFile.open(VolumeFileName);

	if (PointFile.is_open())
		Points.ReadFromFile(PointFile);
	if (VolumeFile.is_open())
		Volume.ReadFromFile(VolumeFile);

	int Done = 0;
	f64 Total = Assets.size();
	for (auto Asset : Assets)
	{
		SciDataParser * Parser = nullptr;
		if (Asset.Parser == "SimpleTXT")
			Parser = new SciDataParserSimpleTXT();
		else if (Asset.Parser == "Grid1")
			Parser = new SciDataParserGrid1();
		else if (Asset.Parser == "CSV")
		{
			SciDataParserCSV * CSV = new SciDataParserCSV();
			CSV->FieldDelim = Asset.FieldDelimiter;
			CSV->ValueDelim = Asset.ValueDelimiter;

			Parser = CSV;
		}

		Parser->DataSet = this;
		Parser->FileName = Site->GetPath() + Asset.File;
		Parser->Load();
		Task->Update(++Done / Total);
	}
}

void CDataSet::ConcurrentLoad()
{
	glGenTextures(1, & VolumeHandle);
	IColorMapper * Mapper = nullptr;

	if (VolumeColorMapper == "Spectrum")
		Mapper = new CSpectrumColorMapper(ColorField);
	else if (VolumeColorMapper == "Oxygen")
		Mapper = new COxygenColorMapper();

	Volume.MakeOpenGLVolume(VolumeHandle, Mapper);


	vec3u Dimensions = Volume.Dimensions; // (10);
	u8 * const VolumeData = new u8[Dimensions.X * Dimensions.Y * Dimensions.Z]();

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

				//f32 MinDistance = 1.0;
				//for (auto Point : Points.GetValues())
				//{
				//	f32 X = (f32) XRange.Normalize(Point.GetField(Traits.PositionXField));
				//	if (XRange.IsEmpty())
				//		X = 0.f;

				//	f32 Y = (f32) YRange.Normalize(Point.GetField(Traits.PositionYField));
				//	if (Traits.InvertY)
				//		Y = 1.f - Y;
				//	if (YRange.IsEmpty())
				//		Y = 0.f;

				//	f32 Z = (f32) ZRange.Normalize(Point.GetField(Traits.PositionZField));
				//	if (ZRange.IsEmpty())
				//		Z = 0.f;

				//	f32 Distance = vec3f(X, Y, Z).GetDistanceSqFrom(vec3f(i / (f32) Dimensions.X, j / (f32) Dimensions.Y, k / (f32) Dimensions.Z));
				//	if (Distance < MinDistance)
				//		MinDistance = Distance;
				//}

				//f64 Value = 1.0 - 24.0 * MinDistance;
				//if (Value < 0)
				//	Value = 0;
				//VolumeData[Index] = Clamp<u32>(Value * 255.0, 0, 255);
				VolumeData[Index] = 255;
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
	Scene.Volume->LoadTextures();


	IColorMapper * Mapper = nullptr;

	if (GlyphColorMapper == "Spectrum")
		Mapper = new CSpectrumColorMapper(ColorField);
	else if (GlyphColorMapper == "Oxygen")
		Mapper = new COxygenColorMapper();

	Scene.Glyphs->LoadGlyphs(this, Mapper);
}

void CDataSet::GenerateVolumeFromPointData()
{
	SRange<f64> XRange = Points.GetFieldRange(Traits.PositionXField, 15.0);
	SRange<f64> YRange = Points.GetFieldRange(Traits.PositionYField, 15.0);
	SRange<f64> ZRange = Points.GetFieldRange(Traits.PositionZField, 15.0);
	SRange<f64> FRange = Points.GetFieldRange(ColorField, 15.0);

	CPrintProgressBar print;
	cout << "Loading interpolator values..." << endl;

	vector<real> Xs, Ys, Zs, Fs;
	int Count = 0;
	print.BeginProgress();
	for (auto Point : Points)
	{
		f64 const X = XRange.Normalize(Point.GetField(Traits.PositionXField));
		f64 const Y = YRange.Normalize(Point.GetField(Traits.PositionYField));
		f64 const Z = ZRange.Normalize(Point.GetField(Traits.PositionZField));
		f64 const F = FRange.Normalize(Point.GetField(ColorField));

		bool Skip = false;

		for (auto & x : Xs)
			if (X == x)
				Skip = true;
		for (auto & y : Ys)
			if (Y == y)
				Skip = true;
		for (auto & z : Zs)
			if (Z == z)
				Skip = true;
		for (auto & f : Fs)
			if (F == f)
				Skip = true;
		
		if (! Skip)
		{
			Xs.push_back(X);
			Ys.push_back(Y);
			Zs.push_back(Z);
			Fs.push_back(F);
		}

		print.SetProgress(++Count / (f32) Points.Size());
	}
	print.EndProgress();
	
	cout << "Creating interpolator..." << endl;
	RBFInterpolator rbfi(Xs, Ys, Zs, Fs);
	cout << "Interpolating..." << endl;
	
	int const Resolution = 28;
	f64 const Scale = Resolution - 1;
	Volume.Dimensions = vec3i(Resolution);
	Volume.Allocate();
	print.BeginProgress();
	for (int k = 0; k < Volume.Dimensions.Z; ++ k)
	{
		for (int j = 0; j < Volume.Dimensions.Y; ++ j)
		{
			for (int i = 0; i < Volume.Dimensions.X; ++ i)
			{
				SVolumeDataRecord<f64> & Row = Volume[i][k][j];
				Row.GetField(ColorField) = rbfi.interpolate(j / Scale, 1.0 - k / Scale, i / Scale);
			}
		}

		print.SetProgress(k / (f32) Volume.Dimensions.Z);
	}
}
