#ifndef _CPROGRAMCONTEXT_H_INCLUDED_
#define _CPROGRAMCONTEXT_H_INCLUDED_

#include "CGUIManager.h"


class CProgramContext : public Singleton<CProgramContext>
{

public:

	CGUIManager * GUIManager;

	void init();
	void run();

};

#endif
