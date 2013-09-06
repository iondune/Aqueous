#include "CTerrainSceneObject.h"

#include "CProgramContext.h"


CTerrainSceneObject::CTerrainSceneObject()
	: Application(CApplication::Get()), SceneManager(CApplication::Get().GetSceneManager()), DebugHeight(false)
{
	setCullingEnabled(false);

	Shader = CProgramContext::Get().Shaders.Terrain;

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
	Flags.Wrap = GL_CLAMP_TO_EDGE;
	ColorMap = CImageLoader::LoadTexture("../Sites/Denmark/TerrainColor.bmp", Flags);
	HeightMap = CImageLoader::LoadTexture("../Sites/Denmark/TerrainTopography.bmp", Flags);
	BathyMap = CImageLoader::LoadTexture("../Sites/Denmark/TerrainBathymetry.bmp", Flags);
}

bool CTerrainSceneObject::draw(IScene const * const Scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled)
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
	Context.uniform("uDebugHeight", DebugHeight ? 1 : 0);

	Context.bindBufferObject("aPosition", VertexData.getHandle(), 2);
	
	if (HeightMap)
		Context.bindTexture("uHeightMap", HeightMap->getTextureHandle());
	if (BathyMap)
		Context.bindTexture("uBathyMap", BathyMap->getTextureHandle());
	if (ColorMap)
		Context.bindTexture("uColorMap", ColorMap->getTextureHandle());

	Context.bindIndexBufferObject(IndexBuffer.getHandle());

	if (isDebugDataEnabled(EDebugData::Wireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, IndexBuffer.size(), GL_UNSIGNED_INT, 0);
	
	if (isDebugDataEnabled(EDebugData::Wireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return true;
}
