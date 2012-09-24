#ifndef _CDATALOADINGTHREAD_H_INCLUDED_
#define _CDATALOADINGTHREAD_H_INCLUDED_

#include "CProgramContext.h"

class CProgramContext;
class CGUILoadingWidget;


class CDataLoadingThread : public sf::Thread
{

private:
	
	virtual void Run();

public:

	CDataLoadingThread();

	CProgramContext * Context;
	std::string FileName;
	CGUILoadingWidget * LoadingWidget;
	bool * Finished;

};

#endif
