#include "CGlyphSceneObject.h"

#include <ionWindow.h>

#include "CProgramContext.h"
#include "CDataSet.h"


CGlyphSceneObject::CGlyphSceneObject()
	: Cube(0), Shader(0), ShowFloors(true), ShowPoints(true), GlyphSize(1 / 64.f)
{
	// No culling!
	setCullingEnabled(false);

	// Setup volume cube
	Cube = CMeshLoader::createCubeMesh();
	Cube->calculateNormalsPerFace();
	Cube->updateBuffers();
	Cube->MeshBuffers[0]->IndexBuffer.syncData();
	Cube->MeshBuffers[0]->NormalBuffer.syncData();
	Cube->MeshBuffers[0]->PositionBuffer.syncData();

	// Copy shader
	Shader = CProgramContext::Get().Shaders.Glyph;
	LineShader = CProgramContext::Get().Shaders.GlyphLines;
}

void CGlyphSceneObject::LoadGlyphs(CDataSet * DataSet, IColorMapper * ColorMapper)
{
	Glyphs.clear();

	ColorMapper->PreProcessValues(DataSet->Points);

	SRange<f64> XRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionXField, 15.0);
	SRange<f64> YRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionYField, 15.0);
	SRange<f64> ZRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionZField, 15.0);

	printf("built in data range is %f %f to %f %f long lat\n", XRange.Minimum, ZRange.Minimum, XRange.Maximum, ZRange.Maximum);
	printf("depth varies from %f to %f\n", YRange.Minimum, YRange.Maximum);

	for (auto Point : DataSet->Points.GetValues())
	{
		SGlyph Glyph;

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

		Glyph.Position = vec3f(X, Y, Z) - 0.5f;
		Glyph.Color = ColorMapper->GetColor(Point);

		Glyphs.push_back(Glyph);
	}

	Context->Scene.Glyphs->BuildLines();
}

bool CGlyphSceneObject::draw(IScene const * const Scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled)
{
	if (! ISceneObject::draw(Scene, Pass, CullingEnabled))
		return false;

	if (Shader)
	{
		CShaderContext Context(* Shader);
		Context.uniform("uModelMatrix", Transformation.getGLMMat4());
		Context.uniform("uNormalMatrix", glm::transpose(glm::inverse(Transformation.getGLMMat4())));
		Context.bindBufferObject("aNormal", Cube->MeshBuffers[0]->NormalBuffer.getHandle(), 3);
		Context.bindBufferObject("aPosition", Cube->MeshBuffers[0]->PositionBuffer.getHandle(), 3);

		Context.uniform("uProjMatrix", SceneManager->getActiveCamera()->GetProjectionMatrix());
		Context.uniform("uViewMatrix", SceneManager->getActiveCamera()->GetViewMatrix());
		Context.uniform("uLightPosition", SceneManager->getActiveCamera()->getPosition());

		Context.uniform("uGlyphSize", GlyphSize);
		Context.uniform("uScale", Scale);

		Context.bindIndexBufferObject(Cube->MeshBuffers[0]->IndexBuffer.getHandle());

		for (auto it = Glyphs.begin(); it != Glyphs.end(); ++ it)
		{
			Context.uniform("uPosition", it->Position);
			Context.uniform("uColor", it->Color);
			glDrawElements(GL_TRIANGLES, Cube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_INT, 0);
		}
	}

	if (LineShader)
	{
		STransformation3 Local;
		Local.setScale(vec3f(1.f/64.f) / Scale);

		CShaderContext Context(* LineShader);
		Context.uniform("uModelMatrix", Transformation.getGLMMat4());
		Context.uniform("uProjMatrix", SceneManager->getActiveCamera()->GetProjectionMatrix());
		Context.uniform("uViewMatrix", SceneManager->getActiveCamera()->GetViewMatrix());

		Context.bindBufferObject("aPosition", Lines.getHandle(), 3);
		Context.bindBufferObject("aColor", LineColors.getHandle(), 3);

		glDrawArrays(GL_LINES, 0, Lines.size() / 3);
	}

	return true;
}

void CGlyphSceneObject::SetFloorsEnabled(bool const showFloors)
{
	ShowFloors = showFloors;
}

void CGlyphSceneObject::SetPointsEnabled(bool const showPoints)
{
	ShowPoints = showPoints;
}

bool CGlyphSceneObject::IsFloorsEnabled()
{
	return ShowFloors;
}

bool CGlyphSceneObject::IsPointsEnabled()
{
	return ShowPoints;
}

void CGlyphSceneObject::BuildLines()
{
	Lines.clear();
	LineColors.clear();

	u32 VertexCount = 0;

	for (u32 i = 1; i < Glyphs.size(); ++ i)
	{
		Lines.push_back(Glyphs[i-1].Position.X);
		Lines.push_back(Glyphs[i-1].Position.Y);
		Lines.push_back(Glyphs[i-1].Position.Z);
		Lines.push_back(Glyphs[i].Position.X);
		Lines.push_back(Glyphs[i].Position.Y);
		Lines.push_back(Glyphs[i].Position.Z);

		LineColors.push_back(Glyphs[i-1].Color.Red);
		LineColors.push_back(Glyphs[i-1].Color.Green);
		LineColors.push_back(Glyphs[i-1].Color.Blue);
		LineColors.push_back(Glyphs[i].Color.Red);
		LineColors.push_back(Glyphs[i].Color.Green);
		LineColors.push_back(Glyphs[i].Color.Blue);
	}

	Lines.syncData();
	LineColors.syncData();
}
