#ifndef _CGLYPHSCENEOBJECT_H_INCLUDED_
#define _CGLYPHSCENEOBJECT_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"

class IColorMapper;


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
	
	void LoadGlyphs(SciDataManager * DataManager, IColorMapper * ColorMapper);

	bool draw(IScene const * const scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled);

	void setShowFloors(bool const showFloors);
	void setShowPoints(bool const showPoints);
	
	bool const getShowFloors();
	bool const getShowPoints();

	void buildLines();

	f32 GlyphSize;

};

#endif
