#ifndef _CGUIWIDGET_H_INCLUDED_
#define _CGUIWIDGET_H_INCLUDED_

#include <ionTypes.h>
#include <ionWindow.h>

#include <Gwen/Controls.h>


class CGUIManager;

class CGUIWidget
{

	CGUIManager * GUIManager;

public:

	CGUIWidget(CGUIManager * Manager);

	virtual void update(float const Elapsed);

};

#endif
