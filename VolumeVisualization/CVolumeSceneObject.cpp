#include "CVolumeSceneObject.h"

#include <ionWindow.h>

#include "CProgramContext.h"


CVolumeSceneObject::SControl::SControl()
	: Mode(0), SliceAxis(1.f, 0.f, 0.f),
	LocalRange(0.2f), MinimumAlpha(0.1f),
	EmphasisLocation(0.5f), AlphaIntensity(1.f),
	StepSize(100.f), DebugLevel(0), UseShading(0)
{}

CVolumeSceneObject::CVolumeSceneObject()
	: Cube(0), Shader(0), SceneManager(CApplication::Get().GetSceneManager())
{
	setCullingEnabled(false);

	// Setup volume cube
	Cube = new CMesh();
    CMesh::SMeshBuffer * Mesh = new CMesh::SMeshBuffer();

    Mesh->Vertices.resize(24);
    Mesh->Vertices[0].Position = SVector3f(-0.5, -0.5, -0.5);
    Mesh->Vertices[1].Position = SVector3f(-0.5,  0.5, -0.5);
    Mesh->Vertices[2].Position = SVector3f( 0.5,  0.5, -0.5);
    Mesh->Vertices[3].Position = SVector3f( 0.5, -0.5, -0.5);
	Mesh->Vertices[0].Color = SColorf(0, 0, 0);
	Mesh->Vertices[1].Color = SColorf(0, 1, 0);
	Mesh->Vertices[2].Color = SColorf(1, 1, 0);
	Mesh->Vertices[3].Color = SColorf(1, 0, 0);

    Mesh->Vertices[4].Position = SVector3f( 0.5, -0.5, -0.5);
    Mesh->Vertices[5].Position = SVector3f( 0.5,  0.5, -0.5);
    Mesh->Vertices[6].Position = SVector3f( 0.5,  0.5,  0.5);
    Mesh->Vertices[7].Position = SVector3f( 0.5, -0.5,  0.5);
	Mesh->Vertices[4].Color = SColorf(1, 0, 0);
	Mesh->Vertices[5].Color = SColorf(1, 1, 0);
	Mesh->Vertices[6].Color = SColorf(1, 1, 1);
	Mesh->Vertices[7].Color = SColorf(1, 0, 1);

    Mesh->Vertices[8].Position = SVector3f(  0.5, -0.5,  0.5);
    Mesh->Vertices[9].Position = SVector3f(  0.5,  0.5,  0.5);
    Mesh->Vertices[10].Position = SVector3f(-0.5,  0.5,  0.5);
    Mesh->Vertices[11].Position = SVector3f(-0.5, -0.5,  0.5);
	Mesh->Vertices[8].Color = SColorf(1, 0, 1);
	Mesh->Vertices[9].Color = SColorf(1, 1, 1);
	Mesh->Vertices[10].Color = SColorf(0, 1, 1);
	Mesh->Vertices[11].Color = SColorf(0, 0, 1);

    Mesh->Vertices[12].Position = SVector3f(-0.5, -0.5,  0.5);
    Mesh->Vertices[13].Position = SVector3f(-0.5,  0.5,  0.5);
    Mesh->Vertices[14].Position = SVector3f(-0.5,  0.5, -0.5);
    Mesh->Vertices[15].Position = SVector3f(-0.5, -0.5, -0.5);
	Mesh->Vertices[12].Color = SColorf(0, 0, 1);
	Mesh->Vertices[13].Color = SColorf(0, 1, 1);
	Mesh->Vertices[14].Color = SColorf(0, 1, 0);
	Mesh->Vertices[15].Color = SColorf(0, 0, 0);

    Mesh->Vertices[16].Position = SVector3f(-0.5,  0.5, -0.5);
    Mesh->Vertices[17].Position = SVector3f(-0.5,  0.5,  0.5);
    Mesh->Vertices[18].Position = SVector3f( 0.5,  0.5,  0.5);
    Mesh->Vertices[19].Position = SVector3f( 0.5,  0.5, -0.5);
	Mesh->Vertices[16].Color = SColorf(0, 1, 0);
	Mesh->Vertices[17].Color = SColorf(0, 1, 1);
	Mesh->Vertices[18].Color = SColorf(1, 1, 1);
	Mesh->Vertices[19].Color = SColorf(1, 1, 0);

    Mesh->Vertices[20].Position = SVector3f( 0.5, -0.5, -0.5);
    Mesh->Vertices[21].Position = SVector3f( 0.5, -0.5,  0.5);
    Mesh->Vertices[22].Position = SVector3f(-0.5, -0.5,  0.5);
    Mesh->Vertices[23].Position = SVector3f(-0.5, -0.5, -0.5);
	Mesh->Vertices[20].Color = SColorf(1, 0, 0);
	Mesh->Vertices[21].Color = SColorf(1, 0, 1);
	Mesh->Vertices[22].Color = SColorf(0, 0, 1);
	Mesh->Vertices[23].Color = SColorf(0, 0, 0);

    Mesh->Triangles.resize(12);
    for (int i = 0; i < 6; ++ i)
    {
        Mesh->Vertices[4*i + 0].TextureCoordinates = SVector2f(0, 1);
        Mesh->Vertices[4*i + 1].TextureCoordinates = SVector2f(0, 0);
        Mesh->Vertices[4*i + 2].TextureCoordinates = SVector2f(1, 0);
        Mesh->Vertices[4*i + 3].TextureCoordinates = SVector2f(1, 1);

        Mesh->Triangles[2*i].Indices[0] = 4*i + 0;
        Mesh->Triangles[2*i].Indices[1] = 4*i + 1;
        Mesh->Triangles[2*i].Indices[2] = 4*i + 2;

        Mesh->Triangles[2*i+1].Indices[0] = 4*i + 0;
        Mesh->Triangles[2*i+1].Indices[1] = 4*i + 2;
        Mesh->Triangles[2*i+1].Indices[2] = 4*i + 3;
    }

    Cube->MeshBuffers.push_back(Mesh);
    Cube->calculateNormalsPerFace();
	Cube->updateBuffers();

	Shader = CProgramContext::Get().Shaders.Volume;
}
	
bool CVolumeSceneObject::draw(IScene const * const Scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled)
{
	if (! ISceneObject::draw(Scene, Pass, CullingEnabled))
		return false;

	// Check data syncing
	if (Cube->MeshBuffers[0]->PositionBuffer.isDirty())
		Cube->MeshBuffers[0]->PositionBuffer.syncData();

	if (Cube->MeshBuffers[0]->ColorBuffer.isDirty())
		Cube->MeshBuffers[0]->ColorBuffer.syncData();

	if (Cube->MeshBuffers[0]->IndexBuffer.isDirty())
		Cube->MeshBuffers[0]->IndexBuffer.syncData();
	
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	int Inversions = 0;
	if (Scale.X < 0)
		Inversions ++;
	if (Scale.Y < 0)
		Inversions ++;
	if (Scale.Z < 0)
		Inversions ++;
	
	if (Inversions % 2)
		glCullFace(GL_BACK);
	else
		glCullFace(GL_FRONT);
	{
		if (Shader)
		{
			CShaderContext Context(* Shader);

			// Attributes
			Context.bindBufferObject("aColor", Cube->MeshBuffers[0]->ColorBuffer.getHandle(), 3);
			Context.bindBufferObject("aPosition", Cube->MeshBuffers[0]->PositionBuffer.getHandle(), 3);
			Context.bindIndexBufferObject(Cube->MeshBuffers[0]->IndexBuffer.getHandle());

			// Matrices
			Context.uniform("uModelMatrix", Transformation.getGLMMat4());
			Context.uniform("uInvModelMatrix", glm::inverse(Transformation.getGLMMat4()));
			Context.uniform("uProjMatrix", SceneManager.getActiveCamera()->GetProjectionMatrix());
			Context.uniform("uViewMatrix", SceneManager.getActiveCamera()->GetViewMatrix());

			// Volume texture
			glEnable(GL_TEXTURE_3D);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_3D, VolumeHandle);
			Context.uniform("uVolumeData", 0);

			// Scene depth
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, CApplication::Get().GetSceneManager().getSceneDepthTexture()->getTextureHandle());
			Context.uniform("uDepthTexture", 1);

			// Control parameters
			Context.uniform("uAlphaIntensity", Control.AlphaIntensity);
			Context.uniform("uHighlightMode", Control.Mode);
			Context.uniform("uSliceAxis", Control.SliceAxis);
			Context.uniform("uLocalRange", Control.LocalRange);
			Context.uniform("uMinimumAlpha", Control.MinimumAlpha);
			Context.uniform("uEmphasisLocation", Control.EmphasisLocation);
			Context.uniform("uStepSize", 1.f / Control.StepSize);
			Context.uniform("uDebugLevel", Control.DebugLevel);
			Context.uniform("uUseShading", Control.UseShading);

			// Camera position for determining front face
			Context.uniform("uCameraPosition", SceneManager.getActiveCamera()->getPosition());
			if (Control.UseShading != 1)
				Context.uniform("uLightPosition", SceneManager.getActiveCamera()->getPosition());
			else
				Context.uniform("uLightPosition", CProgramContext::Get().Scene.OrbitCamera->getPosition());
			
			// Transparency
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Draw
			glDrawElements(GL_TRIANGLES, Cube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_INT, 0);

			// Unbind textures
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_3D, 0);

			// OpenGL state
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_3D);
			glDisable(GL_TEXTURE_2D);
		}
	}
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	return true;
}
