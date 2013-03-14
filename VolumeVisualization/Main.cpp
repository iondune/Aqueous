#include "Libraries.h"

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CProgramContext.h"


int main()
{
	std::cout << "System is initializing..." << std::endl;

	CProgramContext & Context = CProgramContext::get();
	Context.run();

	return 0;
}
