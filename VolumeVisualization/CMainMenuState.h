#ifndef _CMAINMENUSTATE_H_INCLUDED_
#define _CMAINMENUSTATE_H_INCLUDED_

#include "CProgramContext.h"
#include "CDataLoadingThread.h"


class CDataLoadingThread;

class CMainMenuState : public CContextState<CMainMenuState, CProgramContext>
{

	CDataLoadingThread Thread;

public:

	CMainMenuState();

	void Begin();
	void End();
	void Update(f32 const Elapsed);

	void OnEvent(SWindowResizedEvent & Event);

	void loadData(std::string const & FileName);
	void createDataSet();

	std::string DataSetName;

};

#endif
