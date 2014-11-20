
#pragma once
#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"


class CDataSet;

class CGlyph
{
	
public:

	vec3f GetPosition() const;
	color3f GetColor() const;

private:

	friend class CGlyphNodeManager;

	vec3f Position;
	color3f Color;

};

class CGlyphNodeManager : public Singleton<CGlyphNodeManager>, public IEventListener
{

public:

	void Init();
	void LoadGlyphs(CDataSet * DataSet, IColorMapper * ColorMapper);
	void EmitParticles(int Count, vec3f const & Position, color3f const & Color);
	
	CSceneNode * GetNode();
	CSceneNode const * GetNode() const;

protected:

	void LoadSceneElements();

	vector<CGlyph *> Glyphs;
	CSceneNode * Node;
	
	vector<f32> Positions;
	vector<f32> Colors;
	ion::GL::VertexBuffer * PositionBuffer = nullptr;
	ion::GL::VertexBuffer * ColorBuffer = nullptr;

};
