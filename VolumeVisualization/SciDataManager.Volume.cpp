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

f64 const SciDataManager::getGridVolume(std::string const & Field, f64 const Value, f64 const Range) const
{
	unsigned int const size = GridDimensions[0] * GridDimensions[1] * GridDimensions[2] * 4;

	if (GridValues.Values.size() != size / 4)
	{
		printf("Unexpected size of grid data.\n");
		return 0.0;
	}

	f64 TotalSum = 0;

	for (int i = 0; i < GridDimensions[2] - 1; ++ i)
	{
		for (int j = 0; j < GridDimensions[1] - 1; ++ j)
		{
			for (int k = 0; k < GridDimensions[0] - 1; ++ k)
			{
				SciData const **** LocalGrid = new SciData const ***[2];
				double *** IsoValues = new double **[2];
				u8 TruthTable = 0;

				for (int a = 0; a < 2; ++ a)
				{
					LocalGrid[a] = new SciData const **[2];
					IsoValues[a] = new double *[2];
					for (int b = 0; b < 2; ++ b)
					{
						LocalGrid[a][b] = new SciData const *[2];
						IsoValues[a][b] = new double[2];
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
					else if (BitCount == 1)
					{
						// Single corner case

					}
					else if (BitCount == 7)
					{
						// All but single corner
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
		}
	}

	return TotalSum;
}
