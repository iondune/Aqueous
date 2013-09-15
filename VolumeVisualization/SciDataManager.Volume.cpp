#include "SciDataManager.h"


void SciDataManager::createVolumeFromGridValues(IColorMapper * ColorMapper)
{
	unsigned int const size = GridDimensions[0] * GridDimensions[1] * GridDimensions[2] * 4;

	if (GridValues.Size() != size / 4)
	{
		printf("Unexpected size of grid data.\n");
		return;
	}

	ColorMapper->PreProcessValues(GridValues);

	GLubyte * volumeData = new GLubyte[size];
	
	for (u32 k = 0; k < GridDimensions[2]; ++ k)
	for (u32 j = 0; j < GridDimensions[1]; ++ j)
	for (u32 i = 0; i < GridDimensions[0]; ++ i)
	{
		u32 const Index = i + j * GridDimensions[0] + k * GridDimensions[0] * GridDimensions[1];
		color4i const Color = ColorMapper->GetColor(GridValues.GetValues()[Index]);

		for (u32 t = 0; t < 4; ++ t)
			volumeData[Index * 4 + t] = Color[t];
	}


	if (! VolumeHandle)
		glGenTextures(1, & VolumeHandle);
	glBindTexture(GL_TEXTURE_3D, VolumeHandle);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, GridDimensions[0], GridDimensions[2], GridDimensions[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, volumeData);
	
	glBindTexture(GL_TEXTURE_3D, 0);

	delete []volumeData;
}

f64 const InterpolateValues(f64 const * const IsoValues, int i, int j, int k)
{
	int EdgeCount = 0;
				
	EdgeCount += ((i == 1) ? 0 : 1);
	EdgeCount += ((j == 1) ? 0 : 1);
	EdgeCount += ((k == 1) ? 0 : 1);

	if (EdgeCount == 0)
	{
		f64 Sum = 0.0;
		for (int t = 0; t < 8; ++ t)
			Sum += IsoValues[t];
		return Sum / 8.0;
	}
	else if (EdgeCount == 1)
	{
		return (
			IsoValues[4 * (k == 1 ? 0 : k / 2) + 2 * (j == 1 ? 0 : j / 2) + (i == 1 ? 0 : i / 2)] + 
			IsoValues[4 * (k == 1 ? 0 : k / 2) + 2 * (j == 1 ? 1 : j / 2) + (i == 1 ? 1 : i / 2)] + 
			IsoValues[4 * (k == 1 ? 1 : k / 2) + 2 * (j == 1 ? 0 : j / 2) + (i == 1 ? 1 : i / 2)] + 
			IsoValues[4 * (k == 1 ? 1 : k / 2) + 2 * (j == 1 ? 1 : j / 2) + (i == 1 ? 0 : i / 2)]
			) / 4.0;
	}
	else if (EdgeCount == 2)
	{
		return (
			IsoValues[4 * (k == 1 ? 0 : k / 2) + 2 * (j == 1 ? 0 : j / 2) + (i == 1 ? 0 : i / 2)] + 
			IsoValues[4 * (k == 1 ? 1 : k / 2) + 2 * (j == 1 ? 1 : j / 2) + (i == 1 ? 1 : i / 2)]
			) / 2.0;
	}

	std::cout << "Unexpected circumstance!" << std::endl;
	return 0.0;
};

f64 const SciDataManager::getGridVolume(std::string const & Field, f64 const Value, f64 const Range, int const Mode) const
{
	unsigned int const size = GridDimensions[0] * GridDimensions[1] * GridDimensions[2] * 4;

	if (GridValues.Size() != size / 4)
	{
		printf("Unexpected size of grid data.\n");
		return 0.0;
	}

	f64 TotalSum = 0;

	if (Mode == 0 || Mode == 2 || Mode == 3 || Mode == 4)
	{
		f64 * const IsoValues = new f64[8];

		auto const SolveCube = [& TotalSum, Mode](f64 const * const IsoValues, f64 const ScaleFactor) -> void
		{
			int InsideCount = 0;

			for (int a = 0; a < 2; ++ a)
			{
				for (int b = 0; b < 2; ++ b)
				{
					for (int c = 0; c < 2; ++ c)
					{
						int FieldIndex = 4 * c + 2 * b + a;
						if (IsoValues[FieldIndex] >= 0.0)
							InsideCount ++;
					}
				}
			}

					
			if (InsideCount == 0)
				TotalSum += 0.0;
			else if (InsideCount == 8)
				TotalSum += 1.0 * ScaleFactor;
			else
			{
				if (Mode == 0 || Mode == 3)
				{
					TotalSum += (1.0 / 8.0) * InsideCount * ScaleFactor;
				}
				else if (Mode == 2 || Mode == 4)
				{
					for (int a = 0; a < 2; ++ a)
					{
						for (int b = 0; b < 2; ++ b)
						{
							for (int c = 0; c < 2; ++ c)
							{
								int FieldIndex = 4 * c + 2 * b + a;
								double IsoValue = IsoValues[FieldIndex];
								if (IsoValue < 0.0)
									continue;
								
								vec3i Current(a, b, c);
								vec3d Scale;
								for (int i = 0; i < 3; ++ i)
								{
									vec3i Adjacent = Current;
									Adjacent[i] = Adjacent[i] ? 0 : 1;
									int OtherFieldIndex = 4 * Adjacent.Z + 2 * Adjacent.Y + Adjacent.X;

									f64 const OtherIsoValue = IsoValues[OtherFieldIndex];
									if (OtherIsoValue >= 0.0)
										Scale[i] = 0.5;
									else
									{
										double ratio = IsoValue / abs(OtherIsoValue - IsoValue);
										Scale[i] = ratio;
									}
								}
								TotalSum += Scale.X * Scale.Y * Scale.Z * ScaleFactor;
							} // for c
						} // for b
					} // for a
				} // else if (Mode == 2)
			} // else
		};

		std::function<void(f64 const * const, f64 const, int const)> RecurseCube;
		RecurseCube = [& RecurseCube, & SolveCube, & TotalSum](f64 const * const IsoValues, f64 const ScaleFactor, int const Level) -> void
		{
			if (Level <= 0)
				return SolveCube(IsoValues, ScaleFactor);

			int InsideCount = 0;

			for (int a = 0; a < 2; ++ a)
			{
				for (int b = 0; b < 2; ++ b)
				{
					for (int c = 0; c < 2; ++ c)
					{
						int FieldIndex = 4 * c + 2 * b + a;
						if (IsoValues[FieldIndex] >= 0.0)
							InsideCount ++;
					}
				}
			}
					
			if (InsideCount == 0)
			{
				TotalSum += 0.0;
				return;
			}
			else if (InsideCount == 8)
			{
				TotalSum += 1.0 * ScaleFactor;
				return;
			}

			double * NewIsoValues = new double[8];

			for (int a = 0; a < 2; ++ a)
			{
				for (int b = 0; b < 2; ++ b)
				{
					for (int c = 0; c < 2; ++ c)
					{
						int FieldIndex = 4 * c + 2 * b + a;

						for (int d = 0; d < 2; ++ d)
						{
							for (int e = 0; e < 2; ++ e)
							{
								for (int f = 0; f < 2; ++ f)
								{
									int InnerFieldIndex = 4 * f + 2 * e + d;
									NewIsoValues[InnerFieldIndex] = 
										FieldIndex == InnerFieldIndex ? 
										IsoValues[FieldIndex] : 
										InterpolateValues(IsoValues, 
											a + d, 
											b + e, 
											c + f);
								}
							}
						}

						RecurseCube(NewIsoValues, ScaleFactor / 8.0, Level - 1);
					}
				}
			}

			delete NewIsoValues;
		};

		for (u32 i = 0; i < GridDimensions[2] - 1; ++ i)
		{
			for (u32 j = 0; j < GridDimensions[1] - 1; ++ j)
			{
				for (u32 k = 0; k < GridDimensions[0] - 1; ++ k)
				{
					for (u32 a = 0; a < 2; ++ a)
					{
						for (u32 b = 0; b < 2; ++ b)
						{
							for (u32 c = 0; c < 2; ++ c)
							{
								u32 ValueIndex = (k + c) + (j + b) * GridDimensions[0] + (i + a) * GridDimensions[1] * GridDimensions[0];
								u32 FieldIndex = 4 * c + 2 * b + a;
								IsoValues[FieldIndex] = Range - abs(GridValues.GetValues()[ValueIndex].GetField(Field) - Value);
							}
						}
					}

					if (Mode == 0 || Mode == 2)
						SolveCube(IsoValues, 1.0);
					else
						RecurseCube(IsoValues, 1.0, 1);
				}
			}
		}

		delete [] IsoValues;
	}
	else if (Mode == 1)
	{
		for (u32 i = 0; i < GridDimensions[2]; ++ i)
		{
			for (u32 j = 0; j < GridDimensions[1]; ++ j)
			{
				for (u32 k = 0; k < GridDimensions[0]; ++ k)
				{
					u32 ValueIndex = (k + 0) + (j + 0) * GridDimensions[0] + (i + 0) * GridDimensions[1] * GridDimensions[0];
					STable::SRow const * Data = & GridValues.GetValues()[ValueIndex];
					f64 const IsoValue = Range - abs(Data->GetField(Field) - Value);

					if (IsoValue < 0.0)
						continue; //{printf("%f\t", Data->getField(Field)); continue;}
					
					vec3d Scale;

					for (int a = -1; a <= 1; ++ a)
					{
						for (int b = -1; b <= 1; ++ b)
						{
							for (int c = -1; c <= 1; ++ c)
							{
								int ZeroCount = 0;
								int AxisIndex = -1;
								ZeroCount += a ? (AxisIndex = 0, 0) : 1;
								ZeroCount += b ? (AxisIndex = 1, 0) : 1;
								ZeroCount += c ? (AxisIndex = 2, 0) : 1;

								if (ZeroCount == 2)
								{
									
									if ((k + a) < 0 || (k + a) >= GridDimensions[0] ||
										(j + b) < 0 || (j + b) >= GridDimensions[1] ||
										(i + c) < 0 || (i + c) >= GridDimensions[2])
									{
										Scale[AxisIndex] += 0.5;
										continue;
									}

									int ValueIndex = (k + a) + (j + b) * GridDimensions[0] + (i + c) * GridDimensions[1] * GridDimensions[0];
									STable::SRow const * OtherData = & GridValues.GetValues()[ValueIndex];
									f64 const OtherIsoValue = Range - abs(OtherData->GetField(Field) - Value);

									if (OtherIsoValue >= 0.0)
										Scale[AxisIndex] += 0.5;
									else
										Scale[AxisIndex] += IsoValue / abs(OtherIsoValue - IsoValue);
								}
							}
						}
					}

					TotalSum += Scale.X * Scale.Y * Scale.Z;
				}
			}
		}
	}

	return TotalSum;
}

SColorf const ratioToSpectrum(float Ratio)
{
	if (Ratio >= 1.f)
		Ratio = 0.99999f;
	else if (Ratio < 0.f)
		Ratio = 0.f;
	Ratio *= 100.f;
	Ratio /= (100.f / 6.f);
	int i = (int) Ratio;

	float v = 1.f;
	float s = 1.f;
	float ff = Ratio - i;
	float p = v * (1.f - s);
    float q = v * (1.f - (s * ff));
    float t = v * (1.f - (s * (1.f - ff)));

	SColorf out;

	switch(i)
	{
	case 0:
		out.Red = v;
		out.Green = t;
		out.Blue = p;
		break;
	case 1:
		out.Red = q;
		out.Green = v;
		out.Blue = p;
		break;
	case 2:
		out.Red = p;
		out.Green = v;
		out.Blue = t;
		break;
	case 3:
		out.Red = p;
		out.Green = q;
		out.Blue = v;
		break;
	case 4:
		out.Red = t;
		out.Green = p;
		out.Blue = v;
		break;
	case 5:
	default:
		out.Red = v;
		out.Green = 1.f - q;
		out.Blue = 1.f;
		break;
    }

	return out;
}

#include "CProgramContext.h"
#include "CVolumeSceneObject.h"
#include "ProgressPrinter.h"

void SciDataManager::produceVolumeMaps()
{
	ProgressPrinter p;
	p.Begin();

	Range ValueRange = GridValues.GetFieldRange("o1", 5.0);
	CVolumeSceneObject const * const VolumeObject = CProgramContext::Get().Scene.Volume;
	CVolumeSceneObject::SControl const & VolumeControl = VolumeObject->Control;

	for (int i = 4; i < 5; ++ i)
	{
		u32 const ImageSize = 256;
		u8 * const ImageData = new u8[ImageSize * ImageSize * 3];

		for (u32 y = 0; y < ImageSize; ++ y)
		{
			for (u32 x = 0; x < ImageSize; ++ x)
			{
				float const yR = (float) y / (float) (ImageSize - 1);
				float const xR = (float) x / (float) (ImageSize - 1);
				u32 const Index = x * 3 + y * ImageSize * 3;

				float const EmphasisLocation = xR;
				float const LocalRange = yR;

				f64 const GridVolume = getGridVolume("o1", 
					EmphasisLocation * (ValueRange.second - ValueRange.first) + ValueRange.first, 
					LocalRange / 2.f * (ValueRange.second - ValueRange.first), 
					i);

				f32 const VolumeRatio = (f32) GridVolume / (28.f * 23.f * 14.f);

				SColori Color = ratioToSpectrum((f32) VolumeRatio);
				
				ImageData[Index + 0] = Color.Red;
				ImageData[Index + 1] = Color.Green;
				ImageData[Index + 2] = Color.Blue;
			}
			
			p.Update(y * 20 / ImageSize + i * 20);
		}

		CImage * Image = new CImage(ImageData, ImageSize, ImageSize, false);
		std::stringstream s;
		s << "VolumeMapMode" << i << ".bmp";
		Image->Write(s.str());

		delete Image;
	}

	p.End();
	std::cout << ("Volume maps written!") << std::endl;
}
