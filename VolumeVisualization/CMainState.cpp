#include "CMainState.h"

#include "CTerrainSceneObject.h"
#include "CGlyphSceneObject.h"

#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include "libRocket\SystemInterfaceSFML.h"
#include "libRocket\RenderInterfaceSFML.h"
#include "libRocket\ShellFileInterface.h"


CMainState::CMainState()
	: Scale(1), Mode(0), ShowDepth(false)
{}

void CMainState::begin()
{
	//Context->GUIContext->setupMainState();

	Context->Scene.Camera->setProjection(60.f, CApplication::get().getAspectRatio(), 0.01f, 100.f);

	Context->Scene.Timer = 0.f;
	//Context->Scene.GlyphSceneObject->buildLines();



	
	
	/*Renderer = new RocketSFMLRenderer;
	Renderer->SetWindow(CApplication::get().App);
	SystemInterface = new RocketSFMLSystemInterface;
	FileInterface = new ShellFileInterface("libRocket/assets/");

	Rocket::Core::SetFileInterface(FileInterface);
	Rocket::Core::SetRenderInterface(Renderer);
	Rocket::Core::SetSystemInterface(SystemInterface);

	Rocket::Core::Initialise();

	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Bold.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-BoldItalic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Italic.otf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Roman.otf");

	RocketContext = Rocket::Core::CreateContext("default",
		Rocket::Core::Vector2i(CApplication::get().getWindowSize().X, CApplication::get().getWindowSize().Y));

	Rocket::Debugger::Initialise(RocketContext);

	Rocket::Core::ElementDocument* document = RocketContext->LoadDocument("libRocket/tutorial.rml");
	if (document != NULL)
	{
		class EventListener : public Rocket::Core::EventListener
		{

		public:

			virtual void ProcessEvent(Rocket::Core::Event & event)
			{
				std::cout << "Change event!" << std::endl;
			}

		};

		EventListener * eL = new EventListener;

		Rocket::Core::Element * checkbox1 = document->GetElementById("checkbox-1");
		if (checkbox1)
			checkbox1->AddEventListener("change", eL);
		document->Show();
		document->RemoveReference();
	}*/
}

void CMainState::end()
{
	//Context->GUIContext->clear();
}

void CMainState::OnRenderStart(float const Elapsed)
{
	CProgramContext::SScene & Scene = Context->Scene;

	Scene.Camera->update(Elapsed);

	Scene.Timer += Elapsed * 0.16f;

	float const Distance = 4.f;
	Scene.OrbitCamera->setPosition(SVector3f(sin(Scene.Timer)*Distance, 2.3f, cos(Scene.Timer)*Distance));
	Scene.OrbitCamera->setLookAtTarget(SVector3f());

	Scene.LightPosition = SceneManager->getActiveCamera()->getPosition() + SVector3f(0, 0, 0);

	//Scene.LightObject->setTranslation(Scene.LightPosition);
	

	SceneManager->drawAll();
	SceneManager->endDraw();

	if (ShowDepth)
	{
		CFrameBufferObject::bindDeviceBackBuffer();
		glDisable(GL_DEPTH_TEST);
		{
			CShaderContext Context(* CShaderLoader::loadShader("FBO/QuadCopy"));

			Context.bindTexture("uTexColor", SceneManager->getSceneDepthTexture());
			Context.bindBufferObject("aPosition", CSceneManager::getQuadHandle(), 2);

			glDrawArrays(GL_QUADS, 0, 4);
		}
		glEnable(GL_DEPTH_TEST);
	}
	//else
		//Context->Scene.VolumeSceneObject->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);

	//Context->GUIContext->draw(Elapsed, false);

	//RocketContext->Update();
	//RocketContext->Render();
	CApplication::get().swapBuffers();
	if (Scene.Terrain->isVisible())
		Scene.Terrain->DoCameraUpdate = false;
}

void CMainState::OnSFMLEvent(sf::Event const & event)
{
	/*switch(event.type)
	{
	case sf::Event::Resized:
		Renderer->Resize();
		break;
	case sf::Event::MouseMoved:
		RocketContext->ProcessMouseMove(event.mouseMove.x, event.mouseMove.y,
			SystemInterface->GetKeyModifiers(CApplication::get().App));
		break;
	case sf::Event::MouseButtonPressed:
		RocketContext->ProcessMouseButtonDown(event.mouseButton.button,
			SystemInterface->GetKeyModifiers(CApplication::get().App));
		break;
	case sf::Event::MouseButtonReleased:
		RocketContext->ProcessMouseButtonUp(event.mouseButton.button,
			SystemInterface->GetKeyModifiers(CApplication::get().App));
		break;
	case sf::Event::MouseWheelMoved:
		RocketContext->ProcessMouseWheel(event.mouseWheel.delta,
			SystemInterface->GetKeyModifiers(CApplication::get().App));
		break;
	case sf::Event::TextEntered:
		if (event.text.unicode > 32)
			RocketContext->ProcessTextInput(event.text.unicode);
		break;
	case sf::Event::KeyPressed:
		RocketContext->ProcessKeyDown(SystemInterface->TranslateKey(event.key.code),
			SystemInterface->GetKeyModifiers(CApplication::get().App));
		break;
	case sf::Event::KeyReleased:
		if(event.key.code == sf::Keyboard::F8)
		{
			Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
		};

		RocketContext->ProcessKeyUp(SystemInterface->TranslateKey(event.key.code),
			SystemInterface->GetKeyModifiers(CApplication::get().App));
		break;
	case sf::Event::Closed:
		break;
	};*/
}

void CMainState::addConsoleMessage(std::string const & Message, Gwen::Color const & Color)
{
	Context->GUIContext->getConsole()->addMessage(Message, Color);
}
