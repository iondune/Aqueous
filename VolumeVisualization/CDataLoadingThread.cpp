
#include "CDataLoadingThread.h"

#include <ionGUI.h>

#include "CProgramContext.h"
#include "SciDataManager.h"
#include "ColorMappers.h"
#include "CGlyphSceneObject.h"
#include "CMainState.h"


void CDataLoadingThread::Execute()
{
	LoadingWidget->SetProgress(0.05f);
	Context->DataManager->readFromFile(FileName);
	LoadingWidget->SetProgress(0.1f);

	COxygenColorMapper o("d1");
	CSpectrumColorMapper spec("time");
	CSpectrumColorMapper denmark("Chla Conc");
	Context->Scene.Glyphs->LoadGlyphs(Context->DataManager, & denmark);
	LoadingWidget->SetProgress(0.75f);
	LoadingWidget->SetProgress(1.f);
	Executing = false;
}

void CDataLoadingThread::End()
{
	Context->Scene.Glyphs->BuildLines();

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
