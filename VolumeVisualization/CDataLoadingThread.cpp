#include "CDataLoadingThread.h"

#include "CProgramContext.h"
#include "CGUILoadingWidget.h"
#include "SciDataManager.h"
#include "ColorMappers.h"
#include "CGlyphSceneObject.h"
#include "CMainState.h"


static double const pi = 3.14159;
static double const toRadians(double const deg)
{
	return deg * pi / 180;
}

double distFrom(double lat1, double lng1, double lat2, double lng2)
{
	double earthRadius = 3958.75;
	double dLat = toRadians(lat2-lat1);
	double dLng = toRadians(lng2-lng1);
	double a = sin(dLat/2) * sin(dLat/2) +
		cos(toRadians(lat1)) * cos(toRadians(lat2)) *
		sin(dLng/2) * sin(dLng/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double dist = earthRadius * c;

	int meterConversion = 1609;

	return (dist * meterConversion);
}

void CDataLoadingThread::Execute()
{
	LoadingWidget->setProgress(0.05f);
	Context->DataManager->readFromFile(FileName);
	LoadingWidget->setProgress(0.1f);

	int Counter = 0;

	for (auto Data : Context->DataManager->getRawValues().getValues())
	{
		float Progress = Counter++ / (float) Context->DataManager->getRawValues().getValues().size();
		LoadingWidget->setProgress(0.1f + 0.65f * Progress);

		double X = distFrom(Data.getField("Base Latitude"), Data.getField("Base Longitude"), Data.getField("Base Latitude"), Data.getField("End Longitude"));
		double Y = distFrom(Data.getField("Base Latitude"), Data.getField("Base Longitude"), Data.getField("End Latitude"), Data.getField("Base Longitude"));

		Data.getField("End Longitude") = (Data.getField("Base Longitude") > Data.getField("End Longitude") ? 1 : -1) * X;
		Data.getField("End Latitude") = (Data.getField("Base Latitude") > Data.getField("End Latitude") ? 1 : -1) * Y;
	}

	COxygenColorMapper o("d1");
	CSpectrumColorMapper spec("Node Count");
	Context->Scene.GlyphSceneObject->loadGlyphs(Context->DataManager, & spec,
		"End Latitude", "y", "End Longitude", "Total Water Column (m)");
	LoadingWidget->setProgress(0.75f);

	//o.Field = "o1";
	//Context->DataManager->createPointCloudObjects(false, Context->Scene.GridObject, SVector3f(3.f), & o);
	//LoadingWidget->setProgress(1.f);
	Executing = false;
}

void CDataLoadingThread::End()
{
	COxygenColorMapper o("o1");		
	CSpectrumColorMapper spec("Avg Oxy");
	Context->DataManager->createVolumeFromGridValues(& spec);
	Context->Scene.VolumeSceneObject->VolumeHandle = Context->DataManager->getVolumeHandle();
	CApplication::get().getStateManager().setState(& CMainState::get());
		
	Context->Scene.GlyphSceneObject->setScale(vec3f(3.f, 1.5f, 3.f));
}

CDataLoadingThread::CDataLoadingThread()
	: Context(0), LoadingWidget(0), Executing(false), Running(false)
{}

void CDataLoadingThread::Run(std::string const & fileName)
{
	if (! Running)
	{
		Thread = new sf::Thread(& CDataLoadingThread::Execute, this);
		Executing = Running = true;
		FileName = fileName;

		Thread->launch();
	}
}

void CDataLoadingThread::Sync()
{
	if (! Executing && Running)
	{
		End();
		Running = false;
	}
}
