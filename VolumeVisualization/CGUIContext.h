#ifndef _CGUICONTEXT_H_INCLUDED_
#define _CGUICONTEXT_H_INCLUDED_

#include "CGUIManager.h"

#include "CGUIConsoleWidget.h"
#include "CGUITitleLabelsWidget.h"
#include "CGUIVolumeControlWidget.h"


class CMainState;

class CGUIContext : public CGUIManager
{

protected:

	CMainState & MainState;
	
	CGUIConsoleWidget * Console;
	CGUITitleLabelsWidget * TitleLabels;
	CGUIVolumeControlWidget * VolumeControl;

public:

	CGUIContext();

	void setup();
	
	CGUIConsoleWidget * getConsole();
	CGUITitleLabelsWidget * getTitleLabels();
	CGUIVolumeControlWidget * getVolumeControl();

};

#endif
