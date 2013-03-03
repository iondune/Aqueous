#ifndef _SCI_DATA_MANAGER_H_INCLUDED_
#define _SCI_DATA_MANAGER_H_INCLUDED_

#include <vector>
#include <ionScene.h>
#include <ionCore.h>

#include "SciDataCollection.h"
#include "SciDataParser.h"
#include "IColorMapper.h"


class SciDataManager
{

	friend class SciDataParser;

	SciDataCollection RawValues;
	SciDataCollection GridValues;

	unsigned int VolumeHandle;

	SVector3u GridDimensions;

public:

	SciDataManager()
		: VolumeHandle(0), GridDimensions(0)
	{}

	//! Created OpenGL 3d texture using GridValues data collection
	void createVolumeFromGridValues(IColorMapper * ColorMapper);

	//! Interpolate grid data using RBFI
	void createGridDataFromRawValues(Range AcceptedValues = FullRange, double Deviations = 5.0, std::string const & Field = "o2");

	void writeToFile(std::string const & FileName);
	void readFromFile(std::string const & FileName);

	//! Calculate volume of water at a given field value
	f64 const getGridVolume(std::string const & Field, f64 const Value, f64 const Range, int const Mode = 0) const;

	//! Write volume map images (meta-analysis of getGridVolume)
	void produceVolumeMaps();

	
	SciDataCollection & getRawValues();
	SciDataCollection & getGridValues();

	SciDataCollection const & getRawValues() const;
	SciDataCollection const & getGridValues() const;

	unsigned int const getVolumeHandle() const;
	
	SVector3u const & getGridDimensions() const;
	void setGridDimensions(SVector3u const & gridDimensions);

};

#endif
