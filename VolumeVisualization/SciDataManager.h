
#pragma once

#include <vector>
#include <ionScene.h>
#include <ionCore.h>

#include "STable.h"
#include "IColorMapper.h"


class SciDataManager
{

	friend class SciDataParser;

	STable RawValues;
	STable GridValues;

	unsigned int VolumeHandle;

	SVector3u GridDimensions;

public:

	SciDataManager()
		: VolumeHandle(0), GridDimensions(0)
	{}

	//! Created OpenGL 3d texture using GridValues data collection
	void createVolumeFromGridValues(IColorMapper * ColorMapper);
	
	//! Interpolate grid data using RBFI
	void createGridDataFromRawValuesRBFI(Range AcceptedValues = FullRange, double Deviations = 5.0, std::string const & Field = "o2");
	//! Interpolate grid data
	void createGridDataFromRawValues(Range AcceptedValues = FullRange, double Deviations = 5.0, std::string const & Field = "o2");

	void writeToFile(std::string const & FileName);
	void readFromFile(std::string const & FileName);
	void readFromFileLegacy(std::string const & FileName);

	//! Calculate volume of water at a given field value
	f64 const getGridVolume(std::string const & Field, f64 const Value, f64 const Range, int const Mode = 0) const;

	//! Write volume map images (meta-analysis of getGridVolume)
	void produceVolumeMaps();

	
	STable & GetRawValues();
	STable & GetGridValues();

	STable const & GetRawValues() const;
	STable const & GetGridValues() const;

	unsigned int const getVolumeHandle() const;
	
	SVector3u const & getGridDimensions() const;
	void setGridDimensions(SVector3u const & gridDimensions);

};
