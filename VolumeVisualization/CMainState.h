
#pragma once

#include "CProgramContext.h"

#include <CGUIEventManager.h>


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
	}

	void onPreDraw()
	{
		TargetBuffer->bind();

		double plane[4] = {0.0, 1.0, 0.0, 0.0};
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane);
	}

	void onPostDraw()
	{
		glDisable(GL_CLIP_PLANE0);
	}

	CTexture * GetTargetTexture()
	{
		return TargetTexture;
	}

protected:

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

	bool ShowDepth;

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
