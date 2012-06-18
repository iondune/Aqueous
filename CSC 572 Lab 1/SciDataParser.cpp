/***
 * Parser for .log and .oxy files.
 * ---------------------------------
 * author: Ryan Schmitt
 * date: 2-19-11
 */

#include "SciDataParser.h"

char *textFileRead(char const *fn) {
	
	
	FILE *fp;
	char *content = NULL;
	
	int count=0;
	
	if (fn != NULL) {
		fp = fopen(fn,"rt");
		
		if (fp != NULL) {
			
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}


int SciDataParser::parseFile(std::string const &data) {

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

   while(tok != NULL) {

      // scan in line information.  panic if the data does not match
      if(sscanf(tok, "%f %f %f %f %f %f %f", &time, &x, &y, &z, &temp, &O2, &d1) <= 0) {
         printf("Error: Malformed file on line %d\n", line);
         exit(1);
      }

	  if (d1 > maxd1)
		  maxd1 = d1;
	  if (d1 < mind1)
		  mind1 = d1;

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
      m_data.push_back(SciData(x, y, z, O2, temp));
	  m_data.back().d1 = d1;

      // move onto next line
      tok = strtok(NULL, "\n");
      line++;
   }

   printf("smallest d1 is %g and largest is %g.\n", mind1, maxd1);
    // return how many data items were read in
    return m_data.size();

}
