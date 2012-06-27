#define _CRT_SECURE_NO_WARNINGS

#include "SciDataParser.h"

#include "matlib/include/mat.h"

#include <ionScene.h>

int SciDataParser::parseMATGridFile(std::string const &data)
{
	MATFile * File = matOpen(data.c_str(), "r");

	mxArray * var1 = matGetVariable(File, "var1");
	mxArray * var2 = matGetVariable(File, "var2");
	mxArray * var3 = matGetVariable(File, "var3");
	mxArray * var4 = matGetVariable(File, "var4");
	mxArray * pointO1 = matGetVariable(File, "pointO1");
	mxArray * pointO2 = matGetVariable(File, "pointO2");
	mxArray * pointO3 = matGetVariable(File, "pointO3");
	mxArray * pointO4 = matGetVariable(File, "pointO4");
	mxArray * pointX = matGetVariable(File, "pointX");
	mxArray * pointY = matGetVariable(File, "pointY");
	mxArray * pointZ = matGetVariable(File, "pointZ");

	if (mxGetNumberOfDimensions(pointO1) != 3 || 
		mxGetNumberOfDimensions(pointO2) != 3 || 
		mxGetNumberOfDimensions(pointO3) != 3 || 
		mxGetNumberOfDimensions(pointO4) != 3 || 
		mxGetNumberOfDimensions(pointX) != 3 || 
		mxGetNumberOfDimensions(pointY) != 3 || 
		mxGetNumberOfDimensions(pointZ) != 3 || 
		mxGetNumberOfDimensions(var1) != 3 || 
		mxGetNumberOfDimensions(var2) != 3 || 
		mxGetNumberOfDimensions(var3) != 3 || 
		mxGetNumberOfDimensions(var4) != 3)
	{
		printf("Unexpected number of dimensions of input data.\n");
		return -1;
	}

	int const * Dimensions = mxGetDimensions(pointO1);
	double * pointO1Data = mxGetPr(pointO1);
	double * pointO2Data = mxGetPr(pointO2);
	double * pointO3Data = mxGetPr(pointO3);
	double * pointO4Data = mxGetPr(pointO4);
	double * var1Data = mxGetPr(var1);
	double * var2Data = mxGetPr(var2);
	double * var3Data = mxGetPr(var3);
	double * var4Data = mxGetPr(var4);
	double * pointXData = mxGetPr(pointX);
	double * pointYData = mxGetPr(pointY);
	double * pointZData = mxGetPr(pointZ);

	for (int i = 0; i < Dimensions[2]; ++ i)
	{
		for (int j = 0; j < Dimensions[1]; ++ j)
		{
			for (int k = 0; k < Dimensions[0]; ++ k)
			{
				int index = k + j * Dimensions[0] + i * Dimensions[1] * Dimensions[0];

				SciData d(pointXData[index], pointYData[index], pointZData[index]);
				//if (d.ScalarFields["o1"] == 0.0 && )
				//	continue;
				//else
				//	printf("Found a non-thrown-out value!\n");
				d.ScalarFields["o1"] = pointO1Data[index];
				d.ScalarFields["o2"] = pointO2Data[index];
				d.ScalarFields["o3"] = pointO3Data[index];
				d.ScalarFields["o4"] = pointO4Data[index];
				d.ScalarFields["var1"] = var1Data[index];
				d.ScalarFields["var2"] = var2Data[index];
				d.ScalarFields["var3"] = var3Data[index];
				d.ScalarFields["var4"] = var4Data[index];
				Values.push_back(d);
			}
		}
	}

	normalizeField("o1");
	normalizeField("o2");
	normalizeField("o3");
	normalizeField("o4");

	normalizeField("var1");
	normalizeField("var2");
	normalizeField("var3");
	normalizeField("var4");


	/// Generate Volume!
	int size = Dimensions[0]*Dimensions[1]*Dimensions[2]* 4;
	GLubyte * volumeData = new GLubyte[size];

	int ValueIndex = 0;

	for (int i = 0; i < Dimensions[2]; ++ i)
	{
		for (int j = 0; j < Dimensions[1]; ++ j)
		{
			for (int k = 0; k < Dimensions[0]; ++ k)
			{
				int index = k + j * Dimensions[0] + i * Dimensions[1] * Dimensions[0];
				volumeData[index * 4 + 0] = (GLubyte) (Values[ValueIndex].ScalarFields["o2"] * 255.0);
				volumeData[index * 4 + 1] = (GLubyte) (Values[ValueIndex].ScalarFields["o3"] * 255.0);
				volumeData[index * 4 + 2] = (GLubyte) (Values[ValueIndex].ScalarFields["o4"] * 255.0);
				volumeData[index * 4 + 3] = clamp((volumeData[index * 4 + 0] + volumeData[index * 4 + 1] + volumeData[index * 4 + 2]) * 5 / 3, 0 , 255);//200;//(GLubyte) (Values[ValueIndex].ScalarFields["var4"] * 255.0);

				 ++ ValueIndex;
			}
		}
	}


	glEnable(GL_TEXTURE_3D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, & VolumeHandle);
	glBindTexture(GL_TEXTURE_3D, VolumeHandle);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, Dimensions[0], Dimensions[1], Dimensions[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, volumeData);
	
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	delete []volumeData;
	std::cout << "volume texture created " << VolumeHandle << std::endl;

	return Values.size();
}
