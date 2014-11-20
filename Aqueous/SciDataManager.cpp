#include "SciDataManager.h"


STable & SciDataManager::GetRawValues()
{
	return RawValues;
}

STable & SciDataManager::GetGridValues()
{
	return GridValues;
}

STable const & SciDataManager::GetRawValues() const
{
	return RawValues;
}

STable const & SciDataManager::GetGridValues() const
{
	return GridValues;
}

u32 const SciDataManager::getVolumeHandle() const
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
