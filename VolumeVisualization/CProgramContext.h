#ifndef _CPROGRAMCONTEXT_H_INCLUDED_
#define _CPROGRAMCONTEXT_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CGUIManager.h"

#include "SciDataParser.h"
#include "SciDataTree.h"


class CProgramContext : public Singleton<CProgramContext>
{

public:

	CGUIManager * GUIManager;

	SciDataParser * DataParser[3];

	void init();
	void run();

};

#endif
