#include "CGlyphSceneObject.h"

#include <ionWindow.h>

#include "CProgramContext.h"
#include "SciDataManager.h"


CGlyphSceneObject::CGlyphSceneObject()
	: Cube(0), Shader(0), ShowFloors(true), ShowPoints(true)
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
	Shader = CProgramContext::get().Shaders.Glyph;
	LineShader = CProgramContext::get().Shaders.GlyphLines;
}

void CGlyphSceneObject::loadGlyphs(SciDataManager * DataManager, IColorMapper * ColorMapper, 
	std::string const & xField, std::string const & yField, std::string const & zField,
	std::string const & FloorLabel)
{
	Glyphs.clear();

	SciDataCollection & DataSet = DataManager->getRawValues();
	ColorMapper->preProcessValues(DataSet);

	Range XRange = DataSet.getValueRange(xField, 5.0);
	Range YRange = DataSet.getValueRange(yField, 5.0);
	Range ZRange = DataSet.getValueRange(zField, 5.0);

	float MaxSize = max(XRange.second - XRange.first, ZRange.second - ZRange.first);

	for (auto it = DataSet.getValues().begin(); it != DataSet.getValues().end(); ++ it)
	{
		SGlyph g;

		float X = (float) ((it->getField(xField) - XRange.first) / MaxSize);//(XRange.second - XRange.first));
		float Y = 1.f - (float) ((it->getField(yField) - YRange.first) / (YRange.second - YRange.first));//1.f - (it->getField(yField) / YRange.second);
		float Z = (float) ((it->getField(zField) - ZRange.first) / MaxSize);//(ZRange.second - ZRange.first));

		double v = it->getField(FloorLabel);
		if (v != 0.f)
		{
			float Depth = v / YRange.second;
			g.FloorHeight = 1.f - Depth;
		}

		g.Position = vec3f(X, Y, Z);
		g.Color = ColorMapper->getColor(* it);

		Glyphs.push_back(g);
	}
}
	
bool CGlyphSceneObject::draw(IScene const * const Scene, smartPtr<IRenderPass> Pass, bool const CullingEnabled)
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
		
		Context.uniform("uProjMatrix", SceneManager->getActiveCamera()->getProjectionMatrix());
		Context.uniform("uViewMatrix", SceneManager->getActiveCamera()->getViewMatrix());
		Context.uniform("uLightPosition", SceneManager->getActiveCamera()->getPosition());

		Context.bindIndexBufferObject(Cube->MeshBuffers[0]->IndexBuffer.getHandle());
		
		STransformation3 Local;
		Local.setScale(vec3f(1.f/64.f) / Scale);
		for (auto it = Glyphs.begin(); it != Glyphs.end(); ++ it)
		{
			Local.setTranslation((it->Position-vec3f(0.5))*Scale);

			Context.uniform("uLocalMatrix", Local.getGLMMat4());
			Context.uniform("uColor", it->Color);
			glDrawElements(GL_TRIANGLES, Cube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);
		}
		
		Local.setScale(vec3f(3.5f, 0.4f, 3.5f) / 32.f / Scale);
		for (auto it = Glyphs.begin(); it != Glyphs.end(); ++ it)
		{
			if (it->FloorHeight > 1.f)
				continue;

			Local.setTranslation((vec3f(it->Position.X, it->FloorHeight, it->Position.Z)-vec3f(0.5))*Scale);

			Context.uniform("uLocalMatrix", Local.getGLMMat4());
			Context.uniform("uColor", color3f(0.1f, 0.3f, 0.8f));
			glDrawElements(GL_TRIANGLES, Cube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);
		}
	}

	if (LineShader)
	{
		static bool loaded = false;
		static int frame = 1;

		//if (!loaded)
		//	buildLines();
		STransformation3 Local;
		Local.setScale(vec3f(1.f/64.f) / Scale);
		CShaderContext Context(* LineShader);
		Context.uniform("uModelMatrix", Transformation.getGLMMat4());
		Context.uniform("uProjMatrix", SceneManager->getActiveCamera()->getProjectionMatrix());
		Context.uniform("uViewMatrix", SceneManager->getActiveCamera()->getViewMatrix());
		
		Context.bindBufferObject("aPosition", Lines.getHandle(), 3);//Cube->MeshBuffers[0]->PositionBuffer.getHandle(), 3);
		Context.bindBufferObject("aColor", LineColors.getHandle(), 3);//Cube->MeshBuffers[0]->NormalBuffer.getHandle(), 3);
		u32 const ElementCount = Lines.size() / 3 / 16;
		if (!loaded)
			printf("Drawing %d elements\n", ElementCount);

		printf("Drawing frame %d ... ", frame++);
		fflush(stdout);
		try
		{
			//glDrawArrays(GL_LINES, 0, ElementCount);
			printf("Index buffer bound ... ");
			fflush(stdout);
			Context.bindIndexBufferObject(LineIndices.getHandle());
			printf("Drawing elements ... ");
			fflush(stdout);
			glDrawElements(GL_LINES, LineIndices.size(), GL_UNSIGNED_SHORT, 0);
			printf("Done!\n");
		}
		catch (...)
		{
			printf("Caught exception!\n");
		}
		loaded = true;
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
	printf("Lines are building\n");
	LineIndices.clear();
	Lines.clear();
	LineColors.clear();
	LineIndices.setIsIndexBuffer(true);

	u32 VertexCount = 0;

	for (u32 i = 1; i < Glyphs.size(); ++ i)
	{
		LineIndices.push_back(VertexCount++);
		LineIndices.push_back(VertexCount++);
		//LineIndices.push_back(VertexCount++);

		Lines.push_back(Glyphs[i-1].Position.X - 0.1667*Scale.X);
		Lines.push_back(Glyphs[i-1].Position.Y - 0.33*Scale.Y);
		Lines.push_back(Glyphs[i-1].Position.Z - 0.1667*Scale.Z);
		//Lines.push_back(Glyphs[i-2].Position.X - 0.1667*Scale.X);
		//Lines.push_back(Glyphs[i-2].Position.Y - 0.33*Scale.Y);
		//Lines.push_back(Glyphs[i-2].Position.Z - 0.1667*Scale.Z);
		Lines.push_back(Glyphs[i].Position.X - 0.1667*Scale.X);
		Lines.push_back(Glyphs[i].Position.Y - 0.33*Scale.Y);
		Lines.push_back(Glyphs[i].Position.Z - 0.1667*Scale.Z);
		
		LineColors.push_back(Glyphs[i-1].Color.Red);
		LineColors.push_back(Glyphs[i-1].Color.Green);
		LineColors.push_back(Glyphs[i-1].Color.Blue);
		//LineColors.push_back(Glyphs[i-2].Color.Red);
		//LineColors.push_back(Glyphs[i-2].Color.Green);
		//LineColors.push_back(Glyphs[i-2].Color.Blue);
		LineColors.push_back(Glyphs[i].Color.Red);
		LineColors.push_back(Glyphs[i].Color.Green);
		LineColors.push_back(Glyphs[i].Color.Blue);
	}

	for (u32 i = 0; i < Glyphs.size() * 16; ++ i)
	{
		Lines.push_back(0);
		Lines.push_back(0);
		Lines.push_back(0);
		Lines.push_back(0);
		Lines.push_back(0);
		Lines.push_back(0);
		
		LineColors.push_back(0);
		LineColors.push_back(0);
		LineColors.push_back(0);
		LineColors.push_back(0);
		LineColors.push_back(0);
		LineColors.push_back(0);
	}
	
	Lines.syncData();
	LineColors.syncData();
	LineIndices.syncData();
}
