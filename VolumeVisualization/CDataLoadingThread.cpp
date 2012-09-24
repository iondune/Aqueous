#include "CDataLoadingThread.h"

#include "CProgramContext.h"
#include "CGUILoadingWidget.h"
#include "SciDataManager.h"
#include "ColorMappers.h"


void CDataLoadingThread::Run()
{
	LoadingWidget->setProgress(0.25f);
	Context->DataManager->readFromFile(FileName);
	LoadingWidget->setProgress(0.5f);

	COxygenColorMapper o("d1");
	CSpectrumColorMapper spec("\"Avg Oxy\"");
	Context->DataManager->createPointCloudObjects(true, Context->Scene.PointCloudObject, Context->Scene.GridObject, SVector3f(-1.f, 1.f, 1.f), & spec,
		"x", "\"DFS Depth (m)\"", "y");
	LoadingWidget->setProgress(0.75f);

	//o.Field = "o1";
	//Context->DataManager->createPointCloudObjects(false, Context->Scene.GridObject, SVector3f(3.f), & o);
	//LoadingWidget->setProgress(1.f);
	* Finished = true;
}

CDataLoadingThread::CDataLoadingThread()
	: Context(0), LoadingWidget(0), Finished(0)
{}
