#ifndef _CDATALOADINGTHREAD_H_INCLUDED_
#define _CDATALOADINGTHREAD_H_INCLUDED_

#include "CProgramContext.h"

class CProgramContext;
class CGUILoadingWidget;


class CDataLoadingThread
{

	void Execute();
	void Sync();

public:

	CDataLoadingThread();
	void Run();

	CProgramContext * Context;
	CGUILoadingWidget * LoadingWidget;

	std::string FileName;
	bool Finished;

};

#endif
