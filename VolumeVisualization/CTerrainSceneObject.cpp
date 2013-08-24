#include "CTerrainSceneObject.h"

#include "CProgramContext.h"


CTerrainSceneObject::CTerrainSceneObject()
	: Application(CApplication::Get()), SceneManager(CApplication::Get().GetSceneManager())
{
	setCullingEnabled(false);

	Shader = CProgramContext::Get().Shaders.Terrain;

	for (u32 y = 0; y < Size; ++ y)
	{
		for (u32 x = 0; x < Size; ++ x)
		{
			VertexData.push_back(x * 1.f);
			VertexData.push_back(y * 1.f);
		}
	}
	VertexData.syncData();

	IndexBuffer.setIsIndexBuffer(true);
	for (u32 y = 0; y < Size; ++ y)
	{
		for (u32 x = 0; x < Size; ++ x)
		{
			u32 const index = (x + y * Size) * 2;
			IndexBuffer.push_back(index + 0 * 2 + 0 * Size*2);
			IndexBuffer.push_back(index + 1 * 2 + 0 * Size*2);
			IndexBuffer.push_back(index + 1 * 2 + 1 * Size*2);

			IndexBuffer.push_back(index + 0 * 2 + 0 * Size*2);
			IndexBuffer.push_back(index + 1 * 2 + 1 * Size*2);
			IndexBuffer.push_back(index + 0 * 2 + 1 * Size*2);
		}
	}
	IndexBuffer.syncData();
	
	// Create Heightmap Texture
	ColorMap = new CTexture(CImageLoader::loadImage("../TerrainColorImageSquare.bmp"));
	HeightMap = new CTexture(CImageLoader::loadImage("../TerrainHeightImageSquare.bmp"));
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

	Context.bindTexture("uHeightMap", HeightMap->getTextureHandle());
	Context.bindTexture("uColorMap", ColorMap->getTextureHandle());

	Context.bindIndexBufferObject(IndexBuffer.getHandle());

	if (isDebugDataEnabled(EDebugData::Wireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, IndexBuffer.getElements().size(), GL_UNSIGNED_INT, 0);
	
	if (isDebugDataEnabled(EDebugData::Wireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return true;
}
