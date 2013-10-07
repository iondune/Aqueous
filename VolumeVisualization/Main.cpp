
#include "CProgramContext.h"


int main()
{
	std::cout << "System is initializing..." << std::endl;

	CProgramContext & Context = CProgramContext::Get();
	Context.Run();

	return 0;
}
