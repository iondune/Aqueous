#ifndef _CGUIWIDGET_H_INCLUDED_
#define _CGUIWIDGET_H_INCLUDED_

#include <ionTypes.h>
#include <ionWindow.h>

#include <Gwen/Controls.h>


class CGUIManager;

class CGUIWidget : public Gwen::Event::Handler
{

protected:

	CGUIManager * GUIManager;

public:

	CGUIWidget(CGUIManager * Manager);

	virtual void update(f32 const Elapsed);

};

#endif
