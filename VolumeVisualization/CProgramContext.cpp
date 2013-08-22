#include "CProgramContext.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CLoadState.h"


CProgramContext::SScene::SScene()
	: Camera(0), OrbitCamera(0), Timer(0.f),
	LightObject(0), SkyBox(0), Terrain(0), GlyphSceneObject(0), 
	VolumeSceneObject(0), Cube(0)
{}

CProgramContext::SShaders::SShaders()
	: Diffuse(0), DiffuseTexture(0), Volume(0), Terrain(0), Glyph(0), GlyphLines(0)
{}

CProgramContext::CProgramContext()
	: GUIContext(0), DataManager(0)
{}

void CProgramContext::run()
{
	// Directory Setup
	CTextureLoader::ImageDirectory = "Media/";
	CMeshLoader::MeshDirectory = "Media/";
	CShaderLoader::ShaderDirectory = "Shaders/";

	// Create Window
	CApplication & Application = CApplication::Get();
	Application.Init(SVector2i(1900, 1000), "Underwater Volume Data Rendering");

	// Create GUI Engine
	std::cout << "GUI Engine is initializing..." << std::endl;
	GUIContext = new CGUIContext();
	GUIContext->init();

	// Begin loading
	CLoadState & LoadState = CLoadState::Get();
	Application.GetStateManager().SetState(& LoadState);

	// Run program
	Application.Run();
}
