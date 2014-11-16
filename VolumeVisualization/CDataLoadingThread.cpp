
#include "CDataLoadingThread.h"

#include <ionGUI.h>

#include "CProgramContext.h"
#include "SciDataManager.h"
#include "ColorMappers.h"
#include "CGlyphSceneObject.h"
#include "CMainState.h"

#include "CSite.h"


void CDataLoadingThread::Execute()
{
	Context->CurrentSite->Load(LoadingWidget);
	Executing = false;
}

void CDataLoadingThread::End()
{
	Context->CurrentSite->ConcurrentLoad();
	Context->CurrentSite->InitSceneElements(Context->Scene);
	CStateManager::Get().SetState(CMainState::GetPtr());
}

CDataLoadingThread::CDataLoadingThread()
	: Context(0), LoadingWidget(0), Executing(false), Running(false)
{}

void CDataLoadingThread::Run()
{
	if (! Running)
	{
		Executing = Running = true;
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
