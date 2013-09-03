
#include "Libraries.h"
#include "CProgramContext.h"

#include "BathymetyGenerator.h"

int main()
{
	std::cout << "System is initializing..." << std::endl;

	//CProgramContext & Context = CProgramContext::Get();
	//Context.Run();

	BathymetryGenerator BG;
	BG.Generate();

	return 0;
}
