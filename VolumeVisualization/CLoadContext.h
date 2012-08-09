#ifndef _CLOADCONTEXT_H_INCLUDED_
#define _CLOADCONTEXT_H_INCLUDED_

#include "CProgramContext.h"


class CLoadContext : public CContextObject<CProgramContext>
{

protected:

	CGUIManager * GUIManager;
	Gwen::Controls::Canvas * Canvas;

	s32 LabelHeight;

	void addLabel(std::wstring const & Label);

	void loadData();

public:

	CLoadContext();

	void init();
	void run();

};

#endif
