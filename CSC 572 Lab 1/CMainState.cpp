#include "CMainState.h"

CMainState::CMainState()
	: Camera(0), Tyra(0), Scale(1), Mode(3), BindLightPosition(LightPosition)
{}

void CMainState::begin()
{
	init();
	initScene();

	loadData();
}

void CMainState::OnRenderStart(float const Elapsed)
{
	Camera->update(Elapsed);

	Tyra->setTranslation(Translation);
    Tyra->setScale(Scale);
    Tyra->setRotation(Rotation);

	LightPosition = Camera->getPosition() + SVector3f(0, 0, 0);

	LightObject->setTranslation(LightPosition);


	SceneManager.drawAll();
	SceneManager.endDraw();
    SDL_GL_SwapBuffers();
}
