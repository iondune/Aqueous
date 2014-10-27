
#include "CTerrainNodeManager.h"
#include "CProgramContext.h"


CTerrainNodeManager::CTerrainNodeManager()
	: DebugHeight(false), DebugMode(0)
{}

bool CTerrainNodeManager::Load()
{
	Shader = CProgramContext::Get().Shaders.Terrain;
	LayerWidth = Size;

	vector<f32> Vertices;
	for (u32 y = 0; y < HeightmapSize; ++ y)
	{
		for (u32 x = 0; x < HeightmapSize; ++ x)
		{
			Vertices.push_back(x - Size / 2.f);
			Vertices.push_back(y - Size / 2.f);
		}
	}
	VertexData = new CVertexBuffer{};
	VertexData->Data(Vertices, 2);

	vector<u32> Indices;
	for (u32 y = 0; y < Size; ++ y)
	{
		for (u32 x = 0; x < Size; ++ x)
		{
			u32 const index = x + y * HeightmapSize;
			Indices.push_back(index + 0 + 0 * HeightmapSize);
			Indices.push_back(index + 1 + 0 * HeightmapSize);
			Indices.push_back(index + 1 + 1 * HeightmapSize);

			Indices.push_back(index + 0 + 0 * HeightmapSize);
			Indices.push_back(index + 1 + 1 * HeightmapSize);
			Indices.push_back(index + 0 + 1 * HeightmapSize);
		}
	}
	IndexBuffer = new CIndexBuffer{};
	IndexBuffer->Data(Indices);

	Node = SceneManager->GetFactory()->AddSceneNode("Terrain");

	if (Node)
	{
		Node->SetUniform("uModelMatrix", & Node->GetTransformationUniform());
		Node->SetUniform("uLayerWidth", & LayerWidth);
		Node->SetUniform("uDebugHeight", & DebugHeight);
		Node->SetUniform("uDebugMode", & DebugMode);

		Node->SetVertexBuffer("aPosition", VertexData);
		Node->SetIndexBuffer(IndexBuffer);

		if (HeightMap)
			Node->SetTexture("uHeightMap", HeightMap);
		if (ColorMap)
			Node->SetTexture("uColorMap", ColorMap);
		if (BathymetryMap)
			Node->SetTexture("uBathyMap", BathymetryMap);

		return true;
	}

	return false;
}

void CTerrainNodeManager::SetHeightMap(CTexture * heightMap)
{
	HeightMap = heightMap;
	if (Node && HeightMap)
		Node->SetTexture("uHeightMap", HeightMap);
}

void CTerrainNodeManager::SetColorMap(CTexture * colorMap)
{
	ColorMap = colorMap;
	if (Node && ColorMap)
		Node->SetTexture("uColorMap", ColorMap);
}

void CTerrainNodeManager::SetBathymetryMap(CTexture * bathymetryMap)
{
	BathymetryMap = bathymetryMap;
	if (Node && BathymetryMap)
		Node->SetTexture("uBathyMap", BathymetryMap);
}

void CTerrainNodeManager::SetDebugHeightEnabled(bool const Enabled)
{
	DebugHeight = Enabled;
}

bool CTerrainNodeManager::IsDebugHeightEnabled()
{
	return DebugHeight;
}

CSceneNode * CTerrainNodeManager::GetNode()
{
	return Node;
}
