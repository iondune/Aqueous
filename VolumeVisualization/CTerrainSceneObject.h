
#pragma once

#include <ionCore.h>
#include <ionMath.h>
#include <ionScene.h>
#include <ionWindow.h>
#include <ionFramework.h>


class CTerrainSceneObject : public ISceneObject
{

public:

	static u32 const Size = 512;
	static u32 const HeightmapSize = Size + 1;

	CTerrainSceneObject();
	bool draw(IScene const * const scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled);
	
	void SetHeightMap(CTexture * heightMap);
	void SetColorMap(CTexture * colorMap);
	void SetBathymetryMap(CTexture * bathymetryMap);

	void SetDebugHeightEnabled(bool const Enabled);
	bool IsDebugHeightEnabled();

protected:

	CBufferObject<f32> VertexData;
	CBufferObject<u32> IndexBuffer;

	CTexture * HeightMap;
	CTexture * ColorMap;
	CTexture * BathymetryMap;

	CShader * Shader;

	CApplication & Application;
	CSceneManager & SceneManager;

	glm::mat4 ModelMatrix;
	bool DebugHeight;

};
