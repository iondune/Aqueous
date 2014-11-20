
#include "CGlyphNodeManager.h"
#include <ionWindow.h>

#include "CProgramContext.h"
#include "CDataSet.h"


vec3f CGlyph::GetPosition() const
{
	return Position;
}

color3f CGlyph::GetColor() const
{
	return Color;
}


void CGlyphNodeManager::Init()
{
	SingletonPointer<CSceneManager> SceneManager;
	
	Node = SceneManager->GetFactory()->AddSceneNode("Glyph");
}

void CGlyphNodeManager::LoadSceneElements()
{
	size_t const FloatsNeeded = Glyphs.size() * 3;
	PositionBuffer = new ion::GL::VertexBuffer;
	PositionBuffer->Data<f32>(FloatsNeeded * sizeof(f32), nullptr, 3);
	ColorBuffer = new ion::GL::VertexBuffer;
	ColorBuffer->Data<f32>(FloatsNeeded * sizeof(f32), nullptr, 3);
	
	if (Node)
	{
		Node->SetVertexBuffer("vPosition", PositionBuffer);
		Node->SetVertexBuffer("vColor", ColorBuffer);
		Node->SetUniform("Model", & Node->GetTransformationUniform());
		Node->SetPrimitiveType(ion::GL::EPrimitiveType::Points);
	}

	Positions.clear();
	Colors.clear();

	if (Positions.size() < FloatsNeeded)
	{
		Positions.resize(FloatsNeeded);
		Colors.resize(FloatsNeeded);
	}

	for (uint i = 0; i < Glyphs.size(); ++ i)
	{
		Positions[i*3 + 0] = Glyphs[i]->Position.X;
		Positions[i*3 + 1] = Glyphs[i]->Position.Y;
		Positions[i*3 + 2] = Glyphs[i]->Position.Z;
		Colors[i*3 + 0] = Glyphs[i]->Color.Red;
		Colors[i*3 + 1] = Glyphs[i]->Color.Green;
		Colors[i*3 + 2] = Glyphs[i]->Color.Blue;
	}
	
	PositionBuffer->SubData(Positions);
	ColorBuffer->SubData(Colors);

	if (Node)
	{
		Node->SetElementCount((uint) Glyphs.size());
		Node->SetVisible(Glyphs.size() != 0);
	}

}

void CGlyphNodeManager::LoadGlyphs(CDataSet * DataSet, IColorMapper * ColorMapper)
{
	Glyphs.clear();

	ColorMapper->PreProcessValues(DataSet->Points);

	SRange<f64> XRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionXField, 15.0);
	SRange<f64> YRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionYField, 15.0);
	SRange<f64> ZRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionZField, 15.0);

	if (XRange.IsEmpty())
		XRange = SRange<f64>(-1, 1);
	if (YRange.IsEmpty())
		YRange = SRange<f64>(-1, 1);
	if (ZRange.IsEmpty())
		ZRange = SRange<f64>(-1, 1);

	printf("built in data range is %g %g to %g %g long lat\n", XRange.Minimum, ZRange.Minimum, XRange.Maximum, ZRange.Maximum);
	printf("depth varies from %g to %g\n", YRange.Minimum, YRange.Maximum);

	for (auto Point : DataSet->Points.GetValues())
	{
		CGlyph * Glyph = new CGlyph();

		f32 X = (f32) XRange.Normalize(Point.GetField(DataSet->Traits.PositionXField));
		if (XRange.IsEmpty())
			X = 0.f;

		f32 Y = (f32) YRange.Normalize(Point.GetField(DataSet->Traits.PositionYField));
		if (DataSet->Traits.InvertY)
			Y = 1.f - Y;
		if (YRange.IsEmpty())
			Y = 0.f;

		f32 Z = (f32) ZRange.Normalize(Point.GetField(DataSet->Traits.PositionZField));
		if (ZRange.IsEmpty())
			Z = 0.f;

		/*
		f64 v = it->GetField(FloorLabel);
		if (v != 0)
		{
			f32 Depth = (f32) v / (f32) YRange.second;
			Glyph.FloorHeight = 1.f - Depth;
		}
		*/

		Glyph->Position = vec3f(X, Y, Z) - 0.5f;
		Glyph->Color = ColorMapper->GetColor(Point);

		Glyphs.push_back(Glyph);
	}

	LoadSceneElements();
}

CSceneNode * CGlyphNodeManager::GetNode()
{
	return Node;
}

CSceneNode const * CGlyphNodeManager::GetNode() const
{
	return Node;
}
