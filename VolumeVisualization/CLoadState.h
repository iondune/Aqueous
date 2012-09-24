#ifndef _CLOADSTATE_H_INCLUDED_
#define _CLOADSTATE_H_INCLUDED_

#include "CProgramContext.h"

#include <Gwen/Controls/ProgressBar.h>


class CLoadStateEventHandler : public Gwen::Event::Handler
{

public:

	void OnFinish(Gwen::Controls::Base * Control);

};

class CLoadState : public CContextState<CLoadState, CProgramContext>
{

protected:

	CGUIManager * GUIManager;
	Gwen::Controls::Canvas * Canvas;

	s32 LabelHeight, Indent;

	CLoadStateEventHandler Handler;
	
	void loadShaders();
	void loadScene();

	bool GetConfirmation;

public:

	CLoadState();

	void begin();
	void OnRenderStart(float const Elapsed);

	void addLabel(std::wstring const & Label, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 84));
	void OnFinish();

};

#endif
