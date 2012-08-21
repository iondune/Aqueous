#ifndef _CMAINMENUSTATE_H_INCLUDED_
#define _CMAINMENUSTATE_H_INCLUDED_

#include "CProgramContext.h"


class DataLoadThread;


class CMainMenuState : public CContextState<CMainMenuState, CProgramContext>
{

	bool FinishedLoading;
	DataLoadThread * Thread;

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
