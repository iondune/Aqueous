#ifndef _CMAINMENUSTATE_H_INCLUDED_
#define _CMAINMENUSTATE_H_INCLUDED_

#include "CProgramContext.h"


class CMainMenuState : public CContextState<CMainMenuState, CProgramContext>
{

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
