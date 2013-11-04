
#include "CProgramContext.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CLoadState.h"


CProgramContext::SScene::SScene()
	: Camera(), OrbitCamera(), Timer(0),
	LightObject(), SkyBox(), Terrain(), Glyphs(), 
	Volume(), Cube()
{}

CProgramContext::SShaders::SShaders()
	: Diffuse(), DiffuseTexture(), Volume(), Terrain(), Glyph(), GlyphLines()
{}

CProgramContext::CProgramContext()
	: GUIContext(), CurrentSite()
{}

void CProgramContext::Run()
{
	// Directory Setup
	CImageLoader::ImageDirectory = "Media/";
	CMeshLoader::MeshDirectory = "Media/";
	CShaderLoader::ShaderDirectory = "Shaders/";

	// Create Window
	CApplication & Application = CApplication::Get();
	Application.Init(SVector2i(1280, 900), "Underwater Volume Data Rendering");

	// Create GUI Engine
	std::cout << "GUI Engine is initializing..." << std::endl;
	GUIContext = new CGUIContext();
	GUIContext->Init();

	// Begin loading
	CLoadState & LoadState = CLoadState::Get();
	Application.GetStateManager().SetState(& LoadState);

	// Run program
	Application.Run();
}
