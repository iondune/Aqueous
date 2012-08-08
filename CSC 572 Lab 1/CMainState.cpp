#include "CMainState.h"



CMainState::CMainState()
	: Camera(0), Scale(1), Mode(0), BindLightPosition(LightPosition), GUIManager(0), VolumeSceneObject(0)
{
	DataParser[0] = DataParser[1] = DataParser[2] = 0;
}

SVector3f LightPosition;

void CMainState::begin()
{
	init();
	initScene();

	loadData();

	GUIManager->setup();

	Timer = 0.f;
}

void CMainState::loadGUIEngine()
{
	GUIManager = new CGUIManager();
	GUIManager->init();
}

void CMainState::startLoadingContext()
{
	GUIManager->startLoadingContext();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int left = 0, top = 0;
	int right = 1600, bottom = 900;
	glOrtho( left, right, bottom, top, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, right - left, bottom - top);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GUIManager->getCanvas()->RenderCanvas();

	CApplication::get().swapBuffers();
}

#include <iomanip>


CGUIManager & CMainState::getGUIManager()
{
	return * GUIManager;
}

void CMainState::OnRenderStart(float const Elapsed)
{
	Camera->update(Elapsed);

	Timer += Elapsed * 0.16f;

	float const Distance = 4.f;
	OrbitCamera->setPosition(SVector3f(sin(Timer)*Distance, 2.3f, cos(Timer)*Distance));
	OrbitCamera->setLookAtTarget(SVector3f());

	::LightPosition = LightPosition = SceneManager->getActiveCamera()->getPosition() + SVector3f(0, 0, 0);

	LightObject->setTranslation(LightPosition);
	

	SceneManager->drawAll();
	SceneManager->endDraw();


	
	glBindTexture(GL_TEXTURE_2D, 0);
	

	if (GUIManager->ShowGUI)
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



		GUIManager->Console->update(Elapsed);


		GUIManager->getCanvas()->RenderCanvas();
	}

	CApplication::get().swapBuffers();
	Terrain->DoCameraUpdate = false;
}

void CMainState::addConsoleMessage(std::string const & Message, Gwen::Color const & Color)
{
	GUIManager->Console->addMessage(Message, Color);
}
