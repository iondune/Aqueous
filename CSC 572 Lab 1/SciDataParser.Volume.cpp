#include "SciDataParser.h"


void SciDataParser::createVolumeFromGridValues(IColorMapper * ColorMapper)
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

				SColor Color = ColorMapper->getColor(GridValues.Values[ValueIndex]);

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
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, GridDimensions[0], GridDimensions[2], GridDimensions[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, volumeData);
	
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	delete []volumeData;
}
