
#pragma once

#include <ionCore.h>
#include <ionMath.h>
#include <ionScene.h>
#include <ionWindow.h>
#include <ionFramework.h>


class CTerrainSceneObject : public ISceneObject
{

	static u32 const Size = 512;
	static u32 const HeightmapSize = Size + 1;

	// Generic flat grid data, used with heightmap offset
	CBufferObject<f32> VertexData;
	CBufferObject<u32> IndexBuffer;

	CTexture * HeightMap;
	CTexture * ColorMap;

	CShader * Shader;

	CApplication & Application;
	CSceneManager & SceneManager;

	glm::mat4 ModelMatrix;

public:

	CTerrainSceneObject();

	bool draw(IScene const * const scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled);

	bool DebugHeight;

};
