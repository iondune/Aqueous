
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

	for (auto Data : Context->DataManager->GetRawValues().GetValues())
	{
		float Progress = Counter++ / (float) Context->DataManager->GetRawValues().GetValues().size();
		LoadingWidget->setProgress(0.1f + 0.65f * Progress);

		double X = distFrom(Data.GetField("Base Latitude"), Data.GetField("Base Longitude"), Data.GetField("Base Latitude"), Data.GetField("End Longitude"));
		double Y = distFrom(Data.GetField("Base Latitude"), Data.GetField("Base Longitude"), Data.GetField("End Latitude"), Data.GetField("Base Longitude"));

		Data.GetField("End Longitude") = (Data.GetField("Base Longitude") > Data.GetField("End Longitude") ? 1 : -1) * X;
		Data.GetField("End Latitude") = (Data.GetField("Base Latitude") > Data.GetField("End Latitude") ? 1 : -1) * Y;
	}

	COxygenColorMapper o("d1");
	CSpectrumColorMapper spec("time");
	CSpectrumColorMapper denmark("Chla Conc");
	Context->Scene.Glyphs->LoadGlyphs(Context->DataManager, & denmark); //,
		//"Latitude", "DFS Depth", "Longitude", "Total Water Column (m)");
		//"x", "y", "z", "Total Water Column (m)");
	LoadingWidget->setProgress(0.75f);

	//o.Field = "o1";
	//Context->DataManager->createPointCloudObjects(false, Context->Scene.GridObject, SVector3f(3.f), & o);
	LoadingWidget->setProgress(1.f);
	Executing = false;
}

void CDataLoadingThread::End()
{
	Context->Scene.Glyphs->buildLines();
	//Context->Scene.GlyphSceneObject->setScale(vec3f(3.f, 1.5f, 3.f));
	//Context->Scene.GlyphSceneObject->setScale(Context->Scene.GlyphSceneObject->getScale());

	COxygenColorMapper o("o1");
	CSpectrumColorMapper spec("temp");
	CSpectrumColorMapper denmark("Chla Conc");
	Context->DataManager->createVolumeFromGridValues(& denmark);
	Context->Scene.Volume->VolumeHandle = Context->DataManager->getVolumeHandle();
	
	CApplication::Get().GetStateManager().SetState(CMainState::GetPtr());
}

CDataLoadingThread::CDataLoadingThread()
	: Context(0), LoadingWidget(0), Executing(false), Running(false)
{}

void CDataLoadingThread::Run(std::string const & fileName)
{
	if (! Running)
	{
		Executing = Running = true;
		FileName = fileName;
		Thread = new std::thread([](CDataLoadingThread * Thread){Thread->Execute();}, this);
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
