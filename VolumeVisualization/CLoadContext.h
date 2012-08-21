#ifndef _CLOADCONTEXT_H_INCLUDED_
#define _CLOADCONTEXT_H_INCLUDED_

#include "CProgramContext.h"

#include <Gwen/Controls/ProgressBar.h>


class CLoadContext : public CContextObject<CProgramContext>
{

protected:

	CGUIManager * GUIManager;
	Gwen::Controls::Canvas * Canvas;

	s32 LabelHeight, Indent;
	
	void loadShaders();
	void loadScene();

public:

	CLoadContext();

	void init();
	void run();
	
	void addLabel(std::wstring const & Label, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 84));

};

#endif
