#ifndef _CDATALOADINGTHREAD_H_INCLUDED_
#define _CDATALOADINGTHREAD_H_INCLUDED_

#include "CProgramContext.h"

class CProgramContext;
class CGUILoadingWidget;


class CDataLoadingThread
{

	void Execute();
	void End();

	bool Executing;
	bool Running;

	sf::Thread * Thread;

	std::string FileName;

public:

	CDataLoadingThread();
	void Run(std::string const & fileName);
	void Sync();

	CProgramContext * Context;
	CGUILoadingWidget * LoadingWidget;

};

#endif
