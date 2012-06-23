/**
* Class for storing some interesting scientific data
* ---------------------------------
* author: Ryan Schmitt
* date: 2-19-11
*/

#ifndef __SCI_DATA__
#define __SCI_DATA__

#include <string>
#include <ctime>
#include <exception>

#include <SVector3.h>
typedef SVector3f Vector3;


/* Some error definitions */
const int BAD_ARGUMENT_EXCEPTION = 1;

/* The main data class */
class SciData
{
public:
	SciData()
	{}

	SciData(double x, double y, double z, double o2conc, double temp1):temp(temp1),m_o2conc(o2conc),m_loc((float) x, (float)y, (float) z)
	{}

	~SciData()
	{}
	void setLocation(double x, double y, double z);

	Vector3 getLocation() const
	{return m_loc;}

	void setO2Concentration(double concentration)
	{m_o2conc = concentration;}

	double getO2Concenration() const
	{return m_o2conc;}

	void setTempConcentration(double temp1)
	{temp = temp1;}

	double getTempConcenration() const
	{return temp;}

	double d1; // ?

	// void setDateTime(int day, int month, int year, int hour, int min, int sec);
	// tm getDateTime() const {return timestamp;}
	// void setAirSaturation(double saturation) {airSat = saturation;}
	// double getAirSaturation() const {return airSat;}
	// void setTemperature(double temperature) {temp = temperature;}
	// double getTemperature() const {return temp;}

protected:

	Vector3 m_loc;
	double m_o2conc;
	double temp;

	// double airSat, temp;
	// tm timestamp;
};

#include "ionCore.h"
#include <vector>

class ISciTreeNode
{

public:

	SBoundingBox3 Extents;

	virtual bool const isLeaf() const =0;

	virtual ~ISciTreeNode()
	{}

};

enum EOctant
{
	EO_TOP_UPPER_LEFT,
	EO_TOP_UPPER_RIGHT,
	EO_TOP_LOWER_LEFT,
	EO_TOP_LOWER_RIGHT,
	EO_BOTTOM_UPPER_LEFT,
	EO_BOTTOM_UPPER_RIGHT,
	EO_BOTTOM_LOWER_LEFT,
	EO_BOTTOM_LOWER_RIGHT,
	EO_COUNT
};

class CSciTreeNode : public ISciTreeNode
{

public:

	ISciTreeNode * Children[EO_COUNT];

	virtual bool const isLeaf() const
	{
		return false;
	}

};

class CSciTreeLeaf : public ISciTreeNode
{

public:

	std::vector<SciData> Datums;

	virtual bool const isLeaf() const
	{
		return true;
	}

};

#endif
