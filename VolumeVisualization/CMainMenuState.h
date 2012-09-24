#ifndef _CMAINMENUSTATE_H_INCLUDED_
#define _CMAINMENUSTATE_H_INCLUDED_

#include "CProgramContext.h"


class CDataLoadingThread;

class CMainMenuState : public CContextState<CMainMenuState, CProgramContext>
{

	bool FinishedLoading;
	CDataLoadingThread * Thread;

public:

	CMainMenuState();

	void begin();
	void end();

	void OnRenderStart(float const Elapsed);
	void OnWindowResized(SWindowResizedEvent const & Event);

	void loadData(std::string const & FileName);
	void createDataSet();

};

#endif
