
#include "CProgramContext.h"
#include "CGUIContext.h"

#include "CMainState.h"
#include "CMainMenuState.h"
#include "CLoadState.h"

#include <rapidjson/document.h>


void CProgramContext::Run()
{
	SingletonPointer<CWindowManager> WindowManager;
	SingletonPointer<CSceneManager> SceneManager;
	SingletonPointer<CTimeManager> TimeManager;
	SingletonPointer<CStateManager> StateManager;

	// Window Initialization
	WindowManager->Init();
	Window = WindowManager->CreateWindow(vec2i(1920, 1080), "Underwater Volume Data Rendering", EWindowType::Windowed);
	
	// Directory Setup
	SceneManager->GetTextureLibrary()->SetBaseDirectory("Media/");
	SceneManager->GetMeshLibrary()->SetBaseDirectory("Media/");
	SceneManager->GetShaderLibrary()->SetBaseDirectory("Shaders/");

	// Create GUI Engine
	std::cout << "GUI Engine is initializing..." << std::endl;
	SingletonPointer<CGUIContext> GUIContext;
	GUIContext->Init();

	LoadConfig();

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

void CProgramContext::LoadConfig()
{
	rapidjson::Document d;
	d.Parse(File::ReadAsString("./Config.json").c_str());
	
	if (d.HasMember("SitePath") && d["SitePath"].IsString())
		SitePath = d["SitePath"].GetString();
}
