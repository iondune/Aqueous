#ifndef _CGUICONTEXT_H_INCLUDED_
#define _CGUICONTEXT_H_INCLUDED_

#include "CGUIManager.h"
#include "CGUIConsoleWidget.h"


class SciDataManager;
class CMainState;

class CGUIContext : public CGUIManager
{

protected:

	CMainState & MainState;

	Gwen::Controls::Label * VolumeRangeIndicator;
	
	CGUIConsoleWidget * Console;

public:

	CGUIContext();

	void setup();

	void resetVolumeRangeIndicator(SciDataManager * DataManager);
	void clearVolumeRangeIndicator();
	
	CGUIConsoleWidget * getConsole();

};

#endif
