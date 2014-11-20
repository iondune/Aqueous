
#pragma once

#include "CProgramContext.h"
#include "CDataLoadingThread.h"


class CDataLoadingThread;

class CMainMenuState : public CContextState<CMainMenuState, CProgramContext>
{

public:

	CMainMenuState();

	void Begin();
	void End();
	void Update(f32 const Elapsed);

	void OnEvent(SWindowResizedEvent & Event);
	void LoadData();

protected:

	CDataLoadingThread Thread;

};
