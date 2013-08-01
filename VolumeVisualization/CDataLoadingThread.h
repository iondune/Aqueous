#ifndef _CDATALOADINGTHREAD_H_INCLUDED_
#define _CDATALOADINGTHREAD_H_INCLUDED_

#include "CProgramContext.h"

#include <thread>


class CProgramContext;
class CGUILoadingWidget;

class CDataLoadingThread
{

	void Execute();
	void End();

	bool Executing;
	bool Running;

	std::thread * Thread;
	std::string FileName;

public:

	CDataLoadingThread();
	void Run(std::string const & fileName);
	void Sync();

	CProgramContext * Context;
	CGUILoadingWidget * LoadingWidget;

};

#endif
