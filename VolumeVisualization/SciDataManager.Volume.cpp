#include "SciDataManager.h"


void SciDataManager::createVolumeFromGridValues(IColorMapper * ColorMapper)
{
	unsigned int const size = GridDimensions[0] * GridDimensions[1] * GridDimensions[2] * 4;

	if (GridValues.Values.size() != size / 4)
	{
		printf("Unexpected size of grid data.\n");
		return;
	}

	ColorMapper->preProcessValues(GridValues);

	GLubyte * volumeData = new GLubyte[size];

	for (int i = 0; i < GridDimensions[2]; ++ i)
	{
		for (int j = 0; j < GridDimensions[1]; ++ j)
		{
			for (int k = 0; k < GridDimensions[0]; ++ k)
			{
				int index = k + (GridDimensions[2] - i - 1) * GridDimensions[0] + j * GridDimensions[2] * GridDimensions[0];

				int ValueIndex = k + j * GridDimensions[0] + i * GridDimensions[1] * GridDimensions[0];

				SColorAf Color = ColorMapper->getColor(GridValues.Values[ValueIndex]);

				for (int t = 0; t < 4; ++ t)
					volumeData[index * 4 + t] = clamp<unsigned char>((unsigned char) (Color[t] * 255.f), 0, 255);
			}
		}
	}


	glEnable(GL_TEXTURE_3D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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
	glDisable(GL_TEXTURE_3D);

	delete []volumeData;
}

f64 const SciDataManager::getGridVolume(std::string const & Field, f64 const Value, f64 const Range, int const Mode) const
{
	unsigned int const size = GridDimensions[0] * GridDimensions[1] * GridDimensions[2] * 4;

	if (GridValues.Values.size() != size / 4)
	{
		printf("Unexpected size of grid data.\n");
		return 0.0;
	}

	f64 TotalSum = 0;

	if (Mode == 0)
	{
		SciData const **** LocalGrid = new SciData const ***[2];
		double *** IsoValues = new double **[2];

		for (int a = 0; a < 2; ++ a)
		{
			LocalGrid[a] = new SciData const **[2];
			IsoValues[a] = new double *[2];
			for (int b = 0; b < 2; ++ b)
			{
				LocalGrid[a][b] = new SciData const *[2];
				IsoValues[a][b] = new double[2];
			}
		}

		for (int i = 0; i < GridDimensions[2] - 1; ++ i)
		{
			for (int j = 0; j < GridDimensions[1] - 1; ++ j)
			{
				for (int k = 0; k < GridDimensions[0] - 1; ++ k)
				{
					u8 TruthTable = 0;

					for (int a = 0; a < 2; ++ a)
					{
						for (int b = 0; b < 2; ++ b)
						{
							for (int c = 0; c < 2; ++ c)
							{
								int ValueIndex = (k + c) + (j + b) * GridDimensions[0] + (i + a) * GridDimensions[1] * GridDimensions[0];
								int FieldIndex = 4 * c + 2 * b + a;
								LocalGrid[a][b][c] = & GridValues.Values[ValueIndex];
								IsoValues[a][b][c] = Range - abs(LocalGrid[a][b][c]->getField(Field) - Value);
								if (IsoValues[a][b][c] >= 0.0)
									TruthTable |= 1 << FieldIndex;
							}
						}
					}

					if (TruthTable == 0x00)
						TotalSum += 0.0;
					else if (TruthTable == 0xFF)
						TotalSum += 1.0;
					else
					{
						int BitCount = 0;
						for (int t = 0; t < 8; ++ t)
							if (TruthTable & 1 << t)
								BitCount ++;
						if (BitCount == 0 || BitCount == 8)
						{
							// Unexpected...
							std::cerr << "Unexpected but recoverable state: unknown truth table value in grid volume calculation." << std::endl;
							TotalSum += BitCount ? 1.0 : 0.0;
						}
						else
						{
							TotalSum += 1.0 / 8.0;
						}
					}
				}
			}
		}

		for (int a = 0; a < 2; ++ a)
		{
			for (int b = 0; b < 2; ++ b)
			{
				delete [] LocalGrid[a][b];
				delete [] IsoValues[a][b];
			}
			delete [] LocalGrid[a];
			delete [] IsoValues[a];
		}
		delete [] LocalGrid;
		delete [] IsoValues;
	}
	else if (Mode == 1)
	{
		for (int i = 0; i < GridDimensions[2]; ++ i)
		{
			for (int j = 0; j < GridDimensions[1]; ++ j)
			{
				for (int k = 0; k < GridDimensions[0]; ++ k)
				{
					int ValueIndex = (k + 0) + (j + 0) * GridDimensions[0] + (i + 0) * GridDimensions[1] * GridDimensions[0];
					SciData const * Data = & GridValues.Values[ValueIndex];
					f64 const IsoValue = Range - abs(Data->getField(Field) - Value);

					if (IsoValue < 0.0)
						continue; //{printf("%f\t", Data->getField(Field)); continue;}
					
					vec3d Scale;

					for (int a = -1; a <= 1; ++ a)
					{
						for (int b = -1; b <= 1; ++ b)
						{
							for (int c = -1; c <= 1; ++ c)
							{
								if ((k + a) < 0 || (k + a) >= GridDimensions[0])
									continue;
								if ((j + b) < 0 || (j + b) >= GridDimensions[1])
									continue;
								if ((i + c) < 0 || (i + c) >= GridDimensions[2])
									continue;

								int ZeroCount = 0;
								int AxisIndex = -1;
								ZeroCount += a ? (AxisIndex = 0, 0) : 1;
								ZeroCount += b ? (AxisIndex = 1, 0) : 1;
								ZeroCount += c ? (AxisIndex = 2, 0) : 1;

								if (ZeroCount == 2)
								{
									int ValueIndex = (k + a) + (j + b) * GridDimensions[0] + (i + c) * GridDimensions[1] * GridDimensions[0];
									SciData const * OtherData = & GridValues.Values[ValueIndex];
									f64 const OtherIsoValue = Range - abs(OtherData->getField(Field) - Value);

									if (OtherIsoValue >= 0.0)
										Scale[AxisIndex] += 0.5;
									else
									{
										double ratio = IsoValue / abs(OtherIsoValue - IsoValue);
										//if (ratio < 0.0)
										//	ratio += 1.0;
										Scale[AxisIndex] += ratio;
										//printf("ratio %f\t", ratio);
									}
								}
							}
						}
					}

					TotalSum += Scale.X * Scale.Y * Scale.Z;
				}
			}
		}
	}

	return TotalSum ;//* 20 * 20;
}
