
#pragma once

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
	
	void LoadShaders();
	void LoadScene();

	bool GetConfirmation;

public:

	CLoadState();

	void Begin();
	void Update(f32 const Elapsed);

	void AddLabel(std::wstring const & Label, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 84));
	void OnFinish();

};
