#define _CRT_SECURE_NO_WARNINGS

#include "SciDataParser.h"
#include <numeric>

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
				content = new char[count+1];
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}

	return content;
}


int SciDataParser::parseTXTFile(std::string const &data)
{
	// read the files into buffers
	char* dataBuf = textFileRead(data.c_str());

	float time, O2, x, y, z, temp, d1;
	char *tok;
	int line = 0;
	tok = strtok(dataBuf, "\n");

	while(tok != NULL)
	{
		if(sscanf(tok, "%f %f %f %f %f %f %f", &time, &x, &z, &y, &temp, &O2, &d1) <= 0)
		{
			printf("Error: Malformed file on line %d\n", line);
			return -1;
		}

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

	normalizeField("o2");
	normalizeField("temp");
	normalizeField("d1");

	// return how many data items were read in
	return Values.size();
}
