#ifndef _CGUIMANAGER_H_INCLUDED_
#define _CGUIMANAGER_H_INCLUDED_


#include <Gwen/Controls.h>

#include "CConsole.h"

class CApplication;
class CMainState;
class SciDataParser;


class CGUIManager
{

	CApplication & Application;
	CMainState & MainState;

	// GUI Canvas
	Gwen::Controls::Canvas * Canvas;

	Gwen::Controls::Label * VolumeRangeIndicator;
	
	Gwen::Font * LargeFont;
	Gwen::Font * MediumFont;
	Gwen::Font * RegularFont;

public:

	// Settings
	bool ShowGUI;
	
	CConsole * Console;


	CGUIManager();

	void init();
	void setup();
	void startLoadingContext();

	void resetVolumeRangeIndicator(SciDataParser * DataParser);
	void clearVolumeRangeIndicator();

	Gwen::Controls::Canvas * getCanvas()
	{
		return Canvas;
	}

};

#endif
