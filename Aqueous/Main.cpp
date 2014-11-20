
#include "CProgramContext.h"

int main()
{
	std::cout << "System is initializing..." << std::endl;

	SingletonPointer<CProgramContext> Context;
	Context->Run();

	return 0;
}
