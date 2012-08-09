#include "CMainState.h"


CMainState::CMainState()
	: Scale(1), Mode(0)
{}

void CMainState::begin()
{
	Context->GUIManager->setup();

	Context->Scene.Timer = 0.f;
}

#include <iomanip>

void CMainState::OnRenderStart(float const Elapsed)
{
	CProgramContext::SScene & Scene = Context->Scene;

	Scene.Camera->update(Elapsed);

	Scene.Timer += Elapsed * 0.16f;

	float const Distance = 4.f;
	Scene.OrbitCamera->setPosition(SVector3f(sin(Scene.Timer)*Distance, 2.3f, cos(Scene.Timer)*Distance));
	Scene.OrbitCamera->setLookAtTarget(SVector3f());

	Scene.LightPosition = SceneManager->getActiveCamera()->getPosition() + SVector3f(0, 0, 0);

	Scene.LightObject->setTranslation(Scene.LightPosition);
	

	SceneManager->drawAll();
	SceneManager->endDraw();


	
	glBindTexture(GL_TEXTURE_2D, 0);
	

	if (Context->GUIManager->ShowGUI)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glClear(GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		int left = 0, top = 0;
		int right = 1600, bottom = 900;
		glOrtho( left, right, bottom, top, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(0, 0, right - left, bottom - top);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		Context->GUIManager->Console->update(Elapsed);


		Context->GUIManager->getCanvas()->RenderCanvas();
	}

	CApplication::get().swapBuffers();
	Scene.Terrain->DoCameraUpdate = false;
}

void CMainState::addConsoleMessage(std::string const & Message, Gwen::Color const & Color)
{
	Context->GUIManager->Console->addMessage(Message, Color);
}
