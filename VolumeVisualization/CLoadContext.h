#ifndef _CLOADCONTEXT_H_INCLUDED_
#define _CLOADCONTEXT_H_INCLUDED_

#include "CProgramContext.h"

#include <Gwen/Controls/ProgressBar.h>


class CLoadContext : public CContextObject<CProgramContext>
{

protected:

	CGUIManager * GUIManager;
	Gwen::Controls::Canvas * Canvas;
	Gwen::Controls::ProgressBar * Progress;

	s32 LabelHeight, Indent;

	void addLabel(std::wstring const & Label, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 84));
	void setProgress(f32 const Progress);
	
	void loadShaders();
	void loadData();
	void loadScene();

public:

	CLoadContext();

	void init();
	void run();

};

#endif
