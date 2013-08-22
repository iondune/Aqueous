
#include "SciDataParser.h"
#include "SciData.h"
#include "SciDataManager.h"

#include "matlib/include/mat.h"


void SciDataParserCTD::load(std::string const &data)
{
	bool writeCsv = false;

	const char * filename = data.c_str();

	if (false)
	{
		MATFile * pmat;
		const char* name = NULL;
		mxArray * pa;
    
		/* open mat file and read it's content */
		printf("Opening mat file '%s'!\n", filename);
		pmat = matOpen(filename, "r");

		if (pmat == NULL) 
		{
			printf("Error Opening File: \"%s\"\n", "data2.mat");
			return;
		}
		else
			printf("Successfully opened file!\n");
    
		/* Read in each array. */
		pa = matGetNextVariable(pmat, & name);
		while (pa != NULL)
		{
			/*
			* Diagnose array pa
			*/
			int num_of_dim, num_of_field;
			printf("\nArray %s has %d dimensions, %d elements and %d fields.\n", name, num_of_dim = mxGetNumberOfDimensions(pa), mxGetNumberOfElements(pa), num_of_field = mxGetNumberOfFields(pa));
        
			int const * dims = mxGetDimensions(pa);
			for (int i = 0; i < num_of_dim; ++ i)
			{
				printf("Dimension %d has length %d.\n", i + 1, dims[i]);
			}

			for (int i = 0; i < num_of_field; ++ i)
			{
				char const * const fieldname = mxGetFieldNameByNumber(pa, i);
				//printf("Field %d has named %s.\n", i + 1, fieldname);

			
				mxArray * field = mxGetFieldByNumber(pa, 0, i);
				int num_of_dim, num_of_field;
				printf("Field %33s has %d dim, %8d elmnts and %d fld\n", fieldname, num_of_dim = mxGetNumberOfDimensions(field), mxGetNumberOfElements(field), num_of_field = mxGetNumberOfFields(field));
				int const * fieldDims = mxGetDimensions(field);
			}
			//print matrix elements
			//mlfPrintMatrix(pa);

			//mlfPrintMatrix(pa);

			//destroy allocated matrix
			mxDestroyArray(pa);
        
			//get next variable
			pa = matGetNextVariable(pmat, & name);
		}
    
		matClose(pmat);
	}
	else if (true)
	{
		MATFile * File = matOpen(filename, "r");
		FILE * Output;
		
		if (writeCsv)
			Output = fopen("data2.csv", "w");

		const char * name;
		mxArray * ctd = matGetNextVariable(File, & name);

		mxArray * MetaField = mxGetField(ctd, 0, "meta");
		mxArray * sensorUnitsField = mxGetField(MetaField, 0, "sensorUnits");

		int const NumberOfUnitFields = mxGetNumberOfFields(sensorUnitsField);
		for (int i = 0; i < NumberOfUnitFields; ++ i)
		{
			printf("Sensor Unit [%d] = %s\n", i, mxGetFieldNameByNumber(sensorUnitsField, i));
			if (writeCsv)
			{
				fprintf(Output, "%s", mxGetFieldNameByNumber(sensorUnitsField, i));
				if (i != NumberOfUnitFields - 1)
					fprintf(Output, ",");
			}
		}
		if (writeCsv)
			fprintf(Output, "\n");

		mxArray * DataField = mxGetField(ctd, 0, "data");

		int const NumberOfDimensions = mxGetNumberOfDimensions(DataField);
		int const * Dimensions = mxGetDimensions(DataField);

		for (int i = 0; i < NumberOfDimensions; ++ i)
			printf("Dimension[%d] is %d\n", i, Dimensions[i]);

		printf("Data Field class type is %s\n", mxGetClassName(DataField));
		//waitForUser();
		printf("Reading data values from mat.\n");
		if (writeCsv)
			printf("%3d%%", 0);


		double * Data = mxGetPr(DataField);
		int Skip = 1;

		for (int j = 0; j < Dimensions[0]; j += Skip)
		{
			for (int i = 0; i < Dimensions[1]; ++ i)
			{
				int index = j + i * Dimensions[0];
				
				//printf("Trying to access %d %d with index %d\n", i, j, index);
				if (writeCsv)
				{
					fprintf(Output, "%.10e", i == 0 ? 0 : Data[j + (i-1) * Dimensions[0]]);
					if (i != Dimensions[1] - 1)
						fprintf(Output, ",");
				}
				//printf("Value (%d, %d) [%d] is %g\n", i, j, index, Data[index]);
				//waitForUser();
			}
			if (writeCsv)
				fprintf(Output, "\n");


			double Lat = Data[j + 89 * Dimensions[0]];
			double Lon = Data[j + 90 * Dimensions[0]];
			double Depth = Data[j + 86 * Dimensions[0]];
			double Salinty = Data[j + 95 * Dimensions[0]];

			SciData d(Manager->getRawValues());
			d.addField("salinity") = Salinty;
			d.addField("x") = Lat;
			d.addField("y") = Depth;
			d.addField("z") = Lon;

			if (writeCsv)
				printf("\r%3d%%", (int) (100.f * (float) j / (float) (Dimensions[0] - 1.f)));
		}

		if (writeCsv)
		{
			fclose(Output);
			printf("\nDone reading mat.\n");
		}
	}
}

void SciDataParserCTD::examine(std::string const & FileName)
{
	char const * const Indent = "+---";
	MATFile * File = matOpen(FileName.c_str(), "r");

	mxArray * Variable;
	do
	{
		char const * Name;
		Variable = matGetNextVariable(File, & Name);
		if (Variable)
		{
			std::cout << Name << std::endl;

			int const NumberOfDimensions = mxGetNumberOfDimensions(Variable);
			std::cout << Indent << NumberOfDimensions << " dimensions." << std::endl;

			int const * Dimensions = mxGetDimensions(Variable);
			for (int i = 0; i < NumberOfDimensions; ++ i)
				std::cout << Indent << "Dimension[" << i << "] is " << Dimensions[i] << std::endl;

			double * Data = mxGetPr(Variable);

			if (NumberOfDimensions == 2 && Dimensions[0] == 1)
			{
				for (int i = 0; i < Dimensions[1]; ++ i)
					std::cout << Indent << Indent << ' ' << Data[i] << std::endl;
				std::cout << std::endl;
			}
		}
	} while (Variable);
}
