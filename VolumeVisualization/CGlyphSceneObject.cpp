#include "CGlyphSceneObject.h"

#include <ionWindow.h>

#include "CProgramContext.h"
#include "SciDataManager.h"


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

void CGlyphSceneObject::loadGlyphs(SciDataManager * DataManager, IColorMapper * ColorMapper, 
	std::string const & xField, std::string const & yField, std::string const & zField,
	std::string const & FloorLabel)
{
	Glyphs.clear();

	STable & DataSet = DataManager->GetRawValues();
	ColorMapper->PreProcessValues(DataSet);

	Range XRange = DataSet.GetFieldRange(xField, 15.0);
	Range YRange = DataSet.GetFieldRange(yField, 15.0);
	Range ZRange = DataSet.GetFieldRange(zField, 15.0);

	printf("built in data range is %f by %f\n", XRange.second - XRange.first, ZRange.second - ZRange.first);

	bool MaintainXZScale = false;

	for (auto it = DataSet.GetValues().begin(); it != DataSet.GetValues().end(); ++ it)
	{
		SGlyph g;

		f32 MaxField = Max((XRange.second - XRange.first), (ZRange.second - ZRange.first));

		float X = (float) ((it->GetField(xField) - XRange.first) / (MaintainXZScale ? MaxField : (XRange.second - XRange.first)));
		if (XRange.first > XRange.second)
			X = 0.f;

		float Y = 1.f - (float) ((it->GetField(yField) - YRange.first) / (YRange.second - YRange.first));
		if (YRange.first > YRange.second)
			Y = 0.f;

		float Z = (float) ((it->GetField(zField) - ZRange.first) / (MaintainXZScale ? MaxField : (ZRange.second - ZRange.first)));
		if (ZRange.first > ZRange.second)
			Z = 0.f;

		double v = it->GetField(FloorLabel);
		if (v != 0.f)
		{
			f32 Depth = (f32) v / (f32) YRange.second;
			g.FloorHeight = 1.f - Depth;
		}

		g.Position = vec3f(X, Y, Z);
		g.Color = ColorMapper->GetColor(* it);

		Glyphs.push_back(g);
	}
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

void CGlyphSceneObject::setShowFloors(bool const showFloors)
{
	ShowFloors = showFloors;
}

void CGlyphSceneObject::setShowPoints(bool const showPoints)
{
	ShowPoints = showPoints;
}
	
bool const CGlyphSceneObject::getShowFloors()
{
	return ShowFloors;
}

bool const CGlyphSceneObject::getShowPoints()
{
	return ShowPoints;
}

void CGlyphSceneObject::buildLines()
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
