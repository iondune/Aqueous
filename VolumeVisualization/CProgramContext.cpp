
#include "CProgramContext.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CLoadState.h"


void CProgramContext::Run()
{
	SingletonPointer<CWindowManager> WindowManager;
	SingletonPointer<CSceneManager> SceneManager;
	SingletonPointer<CTimeManager> TimeManager;
	SingletonPointer<CStateManager> StateManager;

	// Window Initialization
	WindowManager->Init();
	Window = WindowManager->CreateWindow(vec2i(1600, 1000), "Underwater Volume Data Rendering", EWindowType::Windowed);
	
	// Directory Setup
	SceneManager->GetTextureLibrary()->SetBaseDirectory("Media/");
	SceneManager->GetMeshLibrary()->SetBaseDirectory("Media/");
	SceneManager->GetShaderLibrary()->SetBaseDirectory("Shaders/");

	// Create GUI Engine
	std::cout << "GUI Engine is initializing..." << std::endl;
	SingletonPointer<CGUIContext> GUIContext;
	GUIContext->Init();

	// Begin loading
	SingletonPointer<CLoadState> LoadState;
	StateManager->SetState(LoadState);

	// Run program
	TimeManager->Init();
	while (! WindowManager->ShouldClose())
	{
		StateManager->DoStateChange();

		WindowManager->PollEvents();
		TimeManager->Update();

		StateManager->Update(TimeManager->GetElapsedTime());
		Window->SwapBuffers();
	}
}
