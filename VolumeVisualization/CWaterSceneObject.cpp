
#include "CWaterSceneObject.h"
/*
#include "CProgramContext.h"
#include "CMainState.h"


CWaterSceneObject::CWaterSceneObject()
	: Application(CApplication::Get()), SceneManager(CApplication::Get().GetSceneManager())
{
	setCullingEnabled(false);

	Shader = CProgramContext::Get().Shaders.Water;

	for (u32 y = 0; y < HeightmapSize; ++ y)
	{
		for (u32 x = 0; x < HeightmapSize; ++ x)
		{
			VertexData.push_back(x - Size / 2.f);
			VertexData.push_back(y - Size / 2.f);
		}
	}
	VertexData.syncData();

	IndexBuffer.setIsIndexBuffer(true);
	for (u32 y = 0; y < Size; ++ y)
	{
		for (u32 x = 0; x < Size; ++ x)
		{
			u32 const index = x + y * HeightmapSize;
			IndexBuffer.push_back(index + 0 + 0 * HeightmapSize);
			IndexBuffer.push_back(index + 1 + 0 * HeightmapSize);
			IndexBuffer.push_back(index + 1 + 1 * HeightmapSize);

			IndexBuffer.push_back(index + 0 + 0 * HeightmapSize);
			IndexBuffer.push_back(index + 1 + 1 * HeightmapSize);
			IndexBuffer.push_back(index + 0 + 1 * HeightmapSize);
		}
	}
	IndexBuffer.syncData();
	
	STextureCreationFlags Flags;
	HeightMap = CImageLoader::LoadTexture("../Media/WaterBump.bmp", Flags);
}

bool CWaterSceneObject::draw(IScene const * const Scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled)
{
	if (! ISceneObject::draw(Scene, Pass, CullingEnabled))
		return false;

	// Setup shared shader parameters
	CShaderContext Context(* Shader);
	Context.uniform("uViewMatrix", SceneManager.getActiveCamera()->GetViewMatrix());
	Context.uniform("uProjMatrix", SceneManager.getActiveCamera()->GetProjectionMatrix());
	Context.uniform("uModelMatrix", Transformation.getGLMMat4());
	Context.uniform("uLayerWidth", (f32) Size);
	Context.uniform("uLightPosition", CProgramContext::Get().Scene.LightPosition);
	Context.uniform("uTime", (f32) CApplication::Get().GetRunTime());

	Context.bindBufferObject("aPosition", VertexData.getHandle(), 2);
	Context.bindTexture("uHeightMap", HeightMap->getTextureHandle());
	Context.bindTexture("uReflectMap", CMainState::Get().ReflectionRenderPass->GetTargetTexture());
	Context.bindIndexBufferObject(IndexBuffer.getHandle());

	if (isDebugDataEnabled(EDebugData::Wireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, IndexBuffer.size(), GL_UNSIGNED_INT, 0);
	
	if (isDebugDataEnabled(EDebugData::Wireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return true;
}
*/
