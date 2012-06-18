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

#include "CabbageCore/SVector3.h"
typedef SVector3 Vector3;


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

#endif
