#include "SciDataManager.h"


SciDataCollection & SciDataManager::getRawValues()
{
	return RawValues;
}

SciDataCollection & SciDataManager::getGridValues()
{
	return GridValues;
}

SciDataCollection const & SciDataManager::getRawValues() const
{
	return RawValues;
}

SciDataCollection const & SciDataManager::getGridValues() const
{
	return GridValues;
}

unsigned int const SciDataManager::getVolumeHandle() const
{
	return VolumeHandle;
}

SVector3u const & SciDataManager::getGridDimensions() const
{
	return GridDimensions;
}

void SciDataManager::setGridDimensions(SVector3u const & gridDimensions)
{
	GridDimensions = gridDimensions;
}
