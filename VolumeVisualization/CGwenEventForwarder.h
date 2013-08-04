#ifndef _CGWENEVENTFORWARDER_H_INCLUDED_
#define _CGWENEVENTFORWARDER_H_INCLUDED_


#include <ionWindow.h>
#include <Gwen/Events.h>
#include <Gwen/Controls.h>


class CGwenEventForwarder : public CApplicationEventReceiver
{

	Gwen::Controls::Canvas * Canvas;

	int LastWheel;

public:

	CGwenEventForwarder(Gwen::Controls::Canvas * pCanvas);

	virtual void OnKeyboardEvent(SKeyboardEvent const & Event);

	virtual void OnMouseEvent(SMouseEvent const & Event);

	virtual void OnGameTickStart(float const Elapsed);
	virtual void OnGameTickEnd(float const Elapsed);

	virtual void OnRenderStart(float const Elapsed);
	virtual void OnRenderEnd(float const Elapsed);

	virtual void OnApplicationExit();

};

#endif
