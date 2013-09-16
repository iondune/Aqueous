
#pragma once

#include "CProgramContext.h"

#include <thread>


class CProgramContext;
class CGUIProgressBarWidget;

class CDataLoadingThread
{

	void Execute();
	void End();

	bool Executing;
	bool Running;

	std::thread * Thread;

public:

	CDataLoadingThread();
	void Run();
	void Sync();

	CProgramContext * Context;
	CGUIProgressBarWidget * LoadingWidget;

};
