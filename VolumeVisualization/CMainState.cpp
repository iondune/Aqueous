#include "CMainState.h"

#include "CTerrainSceneObject.h"
#include "CGlyphSceneObject.h"
#include "CWaterSceneObject.h"


CMainState::CMainState()
	: Scale(1), Mode(0), ShowDepth(false)
{}

void CMainState::Begin()
{
	Context->GUIContext->SetupMainState();

	Context->Scene.Timer = 0.f;
	Context->Scene.Glyphs->buildLines();

	ReflectionRenderPass = sharedNew<CReflectionRenderPass>(new CReflectionRenderPass);
	SceneManager->getEffectManager()->RenderPasses.push_back(ReflectionRenderPass);
}

void CMainState::End()
{
	Context->GUIContext->Clear();
}

void CMainState::Update(f32 const Elapsed)
{
	CProgramContext::SScene & Scene = Context->Scene;

	Scene.Camera->Update(Elapsed);

	f32 Radius = Scene.Camera->GetPosition().Length();
	if (Radius < 10.f)
	{
		Scene.Camera->SetNearPlane(0.001f);
		Scene.Camera->SetFarPlane(100.f);
	}
	else if (Radius < 50.f)
	{
		Scene.Camera->SetNearPlane(0.01f);
		Scene.Camera->SetFarPlane(150.f);
	}
	else if (Radius < 500.f)
	{
		Scene.Camera->SetNearPlane(10.f);
		Scene.Camera->SetFarPlane(1000.f);
	}
	else if (Radius < 5000.f)
	{
		Scene.Camera->SetNearPlane(100.f);
		Scene.Camera->SetFarPlane(10000.f);
	}
	else if (Radius < 15000.f)
	{
		Scene.Camera->SetNearPlane(4000.f);
		Scene.Camera->SetFarPlane(20000.f);
	}
	Scene.Camera->UpdateProjection();

	Scene.Timer += Elapsed * 0.16f;

	float const Distance = 4.f;
	Scene.OrbitCamera->setPosition(SVector3f(sin(Scene.Timer)*Distance, 2.3f, cos(Scene.Timer)*Distance));
	Scene.OrbitCamera->SetLookAtTarget(SVector3f());

	Scene.LightPosition = SceneManager->getActiveCamera()->getPosition() + SVector3f(0, 0, 0);

	SceneManager->drawAll();
	//SceneManager->endDraw();

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
		Context->Scene.Volume->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);

	SceneManager->getSceneFrameBuffer()->bind();
	glEnable(GL_BLEND);
	glDepthMask(false);
	Context->Scene.Water->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);
	glDepthMask(true);
	SceneManager->endDraw();

	Context->GUIContext->Draw(Elapsed, false);

	CApplication::Get().GetWindow().SwapBuffers();
}

void CMainState::addConsoleMessage(std::string const & Message, Gwen::Color const & Color)
{
	Context->GUIContext->GetConsole()->addMessage(Message, Color);
}
