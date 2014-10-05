
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
: Diffuse(), DiffuseTexture(), Volume(), Terrain(), Glyph(), GlyphLines(), SkyBox()
{}

CProgramContext::CProgramContext()
	: GUIContext(), CurrentSite(), Window()
{}

void CProgramContext::Run()
{
	SingletonPointer<CWindowManager> WindowManager;
	SingletonPointer<CSceneManager> SceneManager;
	SingletonPointer<CTimeManager> TimeManager;
	SingletonPointer<CStateManager> StateManager;

	WindowManager->Init();
	Window = WindowManager->CreateWindow(vec2i(1600, 1000), "Underwater Volume Data Rendering", EWindowType::Windowed);
	
	SceneManager->GetTextureLibrary()->SetBaseDirectory("Media/");
	SceneManager->GetMeshLibrary()->SetBaseDirectory("Media/");
	SceneManager->GetShaderLibrary()->SetBaseDirectory("Shaders/");

	// Create GUI Engine
	std::cout << "GUI Engine is initializing..." << std::endl;
	GUIContext = new CGUIContext();
	GUIContext->Manager->Init();

	// Begin loading
	CLoadState & LoadState = CLoadState::Get();
	StateManager->SetState(& LoadState);

	// Run program
	ion::GL::Context::Init();
	TimeManager->Init();
	while (! WindowManager->ShouldClose())
	{
		WindowManager->PollEvents();
		TimeManager->Update();

		SceneManager->DrawAll();
		Window->SwapBuffers();
	}
}
