
#pragma once

#include "CProgramContext.h"

#include <glm/gtc/matrix_transform.hpp>


class CReflectionRenderPass : public IRenderPass
{

public:

	CReflectionRenderPass()
	{
		STextureCreationFlags Flags;
		Flags.Filter = GL_LINEAR;
		Flags.MipMaps = false;

		TargetBuffer = new CFrameBufferObject();

		TargetTexture = new CTexture(CApplication::Get().GetWindow().GetSize(), true, Flags);
		TargetBuffer->attachColorTexture(TargetTexture, 0);

		DepthTexture = new CRenderBufferObject(GL_DEPTH_COMPONENT, CApplication::Get().GetWindow().GetSize());
		TargetBuffer->attachDepthRenderBuffer(DepthTexture);

		if (! TargetBuffer->isValid())
			std::cerr << "Target Buffer is not valid" << std::endl;

		printOpenGLErrors("Reflection Render Pass created");

		SceneManager = & CApplication::Get().GetSceneManager();
	}

	void onPreDraw()
	{
		glm::mat4 ViewMatrix = SceneManager->getActiveCamera()->GetViewMatrix();
		ViewMatrix = glm::scale(ViewMatrix, glm::vec3(1, -1, 1));
		SceneManager->getActiveCamera()->SetViewMatrix(ViewMatrix);

		TargetBuffer->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CLIP_PLANE0);
	}

	void onPostDraw()
	{
		glm::mat4 ViewMatrix = SceneManager->getActiveCamera()->GetViewMatrix();
		ViewMatrix = glm::scale(ViewMatrix, glm::vec3(1, -1, 1));
		SceneManager->getActiveCamera()->SetViewMatrix(ViewMatrix);

		glDisable(GL_CLIP_PLANE0);
	}

	CTexture * GetTargetTexture()
	{
		return TargetTexture;
	}

protected:

	CSceneManager * SceneManager;

	CFrameBufferObject * TargetBuffer;
	CTexture * TargetTexture;
	CRenderBufferObject * DepthTexture;

};

class CMainState : public CContextState<CMainState, CProgramContext>
{

public:

	CMainState();

	void Begin();
	void End();

	void Update(f32 const Elapsed);

	void OnEvent(SKeyboardEvent & Event);
	void OnEvent(SWindowResizedEvent & Event);

	void addConsoleMessage(std::string const & Message, Gwen::Color const & Color = Gwen::Color(255, 255, 255, 255));

	void CalculateDataAlignment();

	bool ShowDepth;
	int Site;

	void SetSite(int site);

	//////////////////////////////
	// Mouse Tracking Varaibles //
	//////////////////////////////

	glm::vec3 LastVec;
	int Mode;
	SVector3f Translation, Scale;
	glm::mat4 Rotation;

	sharedPtr<CReflectionRenderPass> ReflectionRenderPass;


	////////////////////////////
	// Mouse Tracking Methods //
	////////////////////////////

	glm::vec3 makeSphereVec(int x, int y);
	void OnEvent(SMouseEvent & Event);

};
