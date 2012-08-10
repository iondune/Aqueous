#ifndef _CGUICONSOLEWIDGET_H_INCLUDED_
#define _CGUICONSOLEWIDGET_H_INCLUDED_

#include <Gwen/Gwen.h>
#include <Gwen/Controls.h>

#include "CGUIWidget.h"


class CGUIConsoleWidget : public CGUIWidget
{

	int Size;
	float Accumulator;
	std::vector<Gwen::Color> MessageColors;
	std::vector<Gwen::Controls::Label *> Messages;
	Gwen::Controls::Canvas * Canvas;

public:

	CGUIConsoleWidget(Gwen::Controls::Canvas * canvas, int const size = 5);

	void setSize(int const size);

	void update(float const Elapsed);

	void addMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255));

};

#endif
