#include "Libraries.h"

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "ContourConverter.h"
#include "CProgramContext.h"


int main()
{
	std::cout << "System is initializing..." << std::endl;

	// Directory Setup
	CTextureLoader::ImageDirectory = "Media/";
	CMeshLoader::MeshDirectory = "Media/";
	CShaderLoader::ShaderDirectory = "Shaders/";

	// Option to run contour converter
	if (false)
	{
		ContourConverter c;
		c.convert();
	}
	else
	{
		CProgramContext & Context = CProgramContext::get();
		Context.init();
		Context.run();
	}

	return 0;
}
