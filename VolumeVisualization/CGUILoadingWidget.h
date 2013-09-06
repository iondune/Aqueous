
#pragma once

#include <Gwen/Gwen.h>
#include <Gwen/Controls.h>
#include <Gwen/Controls/ProgressBar.h>
#include <Gwen/Controls/WindowControl.h>

#include "CGUIContextWidget.h"

#include <ionCore/ionThread.h>


class CGUILoadingWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::ProgressBar * ProgressBar;
	mutex ProgressMutex;

public:

	CGUILoadingWidget(std::string const & Label);

	virtual void setProgress(f32 const Progress);
	virtual void update(f32 const Elapsed);

};
