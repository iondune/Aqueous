#define _CRT_SECURE_NO_WARNINGS

/***
* Parser for .log and .oxy files.
* ---------------------------------
* author: Ryan Schmitt
* date: 2-19-11
*/

#include "SciDataParser.h"

char *textFileRead(char const *fn)
{	
	FILE *fp;
	char *content = NULL;
	int count=0;

	if (fn != NULL)
	{
		fp = fopen(fn,"rt");
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0)
			{
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}

	return content;
}


int SciDataParser::parseFile(std::string const &data)
{
	// read the files into buffers
	char* dataBuf = textFileRead(data.c_str());

	float time, O2, x, y, z, temp, d1;
	char *tok;
	int line = 0;
	tok = strtok(dataBuf, "\n");

	// instantiate beginning min/max values
	m_minLoc = Vector3(FLT_MAX,FLT_MAX,FLT_MAX);
	m_maxLoc = Vector3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
	m_minO2 = DBL_MAX;
	m_maxO2 = -DBL_MAX;

	mind1 = DBL_MAX;
	maxd1 = -DBL_MAX;

	while(tok != NULL)
	{
		if(sscanf(tok, "%f %f %f %f %f %f %f", &time, &x, &z, &y, &temp, &O2, &d1) <= 0)
		{
			printf("Error: Malformed file on line %d\n", line);
			return -1;
		}

		y *= 25.0;

		if (d1 > maxd1)	maxd1 = d1;
		if (d1 < mind1)	mind1 = d1;

		// update minimum location
		if(x < m_minLoc.X) m_minLoc.X = x;
		if(y < m_minLoc.Y) m_minLoc.Y = y;
		if(z < m_minLoc.Z) m_minLoc.Z = z;

		// update maximum location
		if(x > m_maxLoc.X) m_maxLoc.X = x;
		if(y > m_maxLoc.Y) m_maxLoc.Y = y;
		if(z > m_maxLoc.Z) m_maxLoc.Z = z;

		// update min/max O2 values
		if(O2 < m_minO2) m_minO2 = O2;
		if(O2 > m_maxO2) m_maxO2 = O2;

		// push scidata into list
		SciData d(x, y, z);
		d.ScalarFields["o2"] = O2;
		d.ScalarFields["temp"] = temp;
		d.ScalarFields["d1"] = d1;
		Values.push_back(d);

		// move onto next line
		tok = strtok(NULL, "\n");
		line++;
	}

	// return how many data items were read in
	return Values.size();

}
