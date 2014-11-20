
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

public:

	CLoadState();

	void Begin();
	void Update(f32 const Elapsed);

	void AddLabel(std::wstring const & Label, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 84));
	void OnFinish();

protected:

	SingletonPointer<CGUIManager> GUIManager;
	Gwen::Controls::Canvas * Canvas = nullptr;

	s32 LabelHeight = 0, Indent = 0;

	CLoadStateEventHandler Handler;
	
	void LoadShaders();
	void LoadScene();

	bool GetConfirmation = false;

};
