#ifndef _CTERRAINSCENEOBJECT_H_INCLUDED_
#define _CTERRAINSCENEOBJECT_H_INCLUDED_

#include <ionCore.h>
#include <ionMath.h>
#include <ionScene.h>
#include <ionWindow.h>
#include <ionFramework.h>


class CTerrainSceneObject : public ISceneObject
{

	static int const Size = 512;
	static int const HeightmapSize = Size + 1;
	static int const LayerCount = 1;

	// Generic flat grid data, used with heightmap offset
	CBufferObject<float> VertexData;

	class SLayer
	{

	public:

		CBufferObject<unsigned int> IndexBuffer;

		float HostHeightMap[HeightmapSize][HeightmapSize];

		CTexture * HeightMap;
		CTexture * ColorMap;

		STransformation3 Transformation;
		int Level, ScaleFactor;
		bool Active;

		SRect2i ActiveRegion, ClipRegion;
		SVector2i DataOffset;

		SLayer(int const i);

		int sendSample(int const x1, int const y1, int const x2, int const y2, SVector2i const & NewClipPos);

	};

	std::vector<SLayer *> Layers;

	CShader * Shader;

	CApplication & Application;
	CSceneManager & SceneManager;

	glm::mat4 ModelMatrix;

public:

	int DrawLevel;

	bool DoCameraUpdate;
	bool DebugHeight;

	CTerrainSceneObject();

	float const getTerrainHeight(SVector2f const & Position);

	bool draw(IScene const * const scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled);

};

#endif
