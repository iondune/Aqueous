
#pragma once
/*
#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"

class IColorMapper;
class CDataSet;


class CGlyphSceneObject : public ISceneObject, public CApplicationContextObject<CProgramContext>
{

	// Cube used for rendering each glyph
	CMesh * Cube;

	CBufferObject<f32> Lines, LineColors;

	// Shader for rendering each cube
	CShader * Shader;
	CShader * LineShader;

	struct SGlyph
	{
		color3f Color;
		vec3f Position;
		float FloorHeight;

		SGlyph()
			: FloorHeight(100.f)
		{}
	};

	std::vector<SGlyph> Glyphs;
	bool ShowFloors;
	bool ShowPoints;

public:

	CGlyphSceneObject();
	
	void LoadGlyphs(CDataSet * DataSet, IColorMapper * ColorMapper);

	bool draw(IScene const * const scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled);

	void SetFloorsEnabled(bool const showFloors);
	void SetPointsEnabled(bool const showPoints);
	
	bool IsFloorsEnabled();
	bool IsPointsEnabled();

	void BuildLines();

	f32 GlyphSize;

};
*/
