#ifndef _CGUICONSOLEWIDGET_H_INCLUDED_
#define _CGUICONSOLEWIDGET_H_INCLUDED_

#include <Gwen/Gwen.h>
#include <Gwen/Controls.h>

#include "CGUIContextWidget.h"


class CGUIConsoleWidget : public CGUIContextWidget
{

	int Size;
	float Accumulator;
	std::vector<Gwen::Color> MessageColors;
	std::vector<Gwen::Controls::Label *> Messages;

public:

	CGUIConsoleWidget(int const size = 5);

	void setSize(int const size);

	void update(float const Elapsed);

	void addMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255));

};

#endif
