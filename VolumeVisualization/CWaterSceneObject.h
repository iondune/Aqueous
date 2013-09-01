
#pragma once

#include <ionCore.h>
#include <ionMath.h>
#include <ionScene.h>
#include <ionWindow.h>
#include <ionFramework.h>


class CWaterSceneObject : public ISceneObject
{

public:

	CWaterSceneObject();

	bool draw(IScene const * const scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled);

protected:
	
	static u32 const Size = 512;
	static u32 const HeightmapSize = Size + 1;

	CApplication & Application;
	CSceneManager & SceneManager;

	glm::mat4 ModelMatrix;

	CBufferObject<f32> VertexData;
	CBufferObject<u32> IndexBuffer;

	CTexture * HeightMap;
	CShader * Shader;

};
