#ifndef _CGUICONSOLEWIDGET_H_INCLUDED_
#define _CGUICONSOLEWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"


class CGUIConsoleWidget : public CGUIContextWidget
{

	int Size;
	float Accumulator;
	//std::vector<Gwen::Color> MessageColors;
	//std::vector<Gwen::Controls::Label *> Messages;

public:

	CGUIConsoleWidget(int const size = 5);

	virtual void setSize(int const size);
	virtual void update(f32 const Elapsed);
	//virtual void addMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255));

};

#endif
