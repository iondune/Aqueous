#include "CMainState.h"

#include "CTerrainSceneObject.h"


CMainState::CMainState()
	: Scale(1), Mode(0), ShowDepth(false)
{}

void CMainState::begin()
{
	Context->GUIContext->setupMainState();

	Context->Scene.Camera->setProjection(60.f, CApplication::get().getAspectRatio(), 0.01f, 100.f);

	Context->Scene.Timer = 0.f;
}

void CMainState::end()
{
	Context->GUIContext->clear();
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
	// Draw to screen
	/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// THE FINAL RENDER
	{
		CShaderContext Context(* CShaderLoader::loadShader("FBO/QuadCopy"));

		if (ShowDepth)
			Context.bindTexture("uTexColor", SceneManager->getSceneDepthTexture());
		else
			Context.bindTexture("uTexColor", SceneManager->getSceneFrameTexture());
		Context.bindBufferObject("aPosition", CSceneManager::getQuadHandle(), 2);

		glDrawArrays(GL_QUADS, 0, 4);
	}

	if (! ShowDepth)
		Context->Scene.VolumeSceneObject->draw(SceneManager, ERenderPass::Default, false);

	glEnable(GL_DEPTH_TEST);*/

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
	else
		Context->Scene.VolumeSceneObject->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);

	Context->GUIContext->draw(Elapsed, false);

	CApplication::get().swapBuffers();
	if (Scene.Terrain->isVisible())
		Scene.Terrain->DoCameraUpdate = false;
}

void CMainState::addConsoleMessage(std::string const & Message, Gwen::Color const & Color)
{
	Context->GUIContext->getConsole()->addMessage(Message, Color);
}
