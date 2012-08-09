#include "CVolumeSceneObject.h"

#include <ionWindow.h>


CVolumeSceneObject::CVolumeSceneObject()
	: VolumeCube(0), VolumeBuffer(0), VolumeTarget(0), ShowVolume(0), SceneManager(CApplication::get().getSceneManager())
{
	setCullingEnabled(false);

	// Setup volume cube
	VolumeCube = new CMesh();
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

    VolumeCube->MeshBuffers.push_back(Mesh);
    VolumeCube->calculateNormalsPerFace();

	VolumeCube->updateBuffers();


	VolumeTarget = new CFrameBufferObject();

	STextureCreationFlags Flags;
	Flags.Filter = GL_LINEAR;
	Flags.MipMaps = false;
	Flags.Wrap = GL_MIRRORED_REPEAT;
	VolumeBuffer = new CTexture(SceneManager.getScreenSize(), true, Flags);
	VolumeTarget->attach(VolumeBuffer, GL_COLOR_ATTACHMENT0);
}
	
bool CVolumeSceneObject::draw(IScene const * const Scene, ERenderPass const Pass, bool const CullingEnabled)
{
	if (! ISceneObject::draw(Scene, Pass, CullingEnabled))
		return false;

	// Check data syncing
	if (VolumeCube->MeshBuffers[0]->PositionBuffer.isDirty())
		VolumeCube->MeshBuffers[0]->PositionBuffer.syncData();

	if (VolumeCube->MeshBuffers[0]->ColorBuffer.isDirty())
		VolumeCube->MeshBuffers[0]->ColorBuffer.syncData();

	if (VolumeCube->MeshBuffers[0]->IndexBuffer.isDirty())
		VolumeCube->MeshBuffers[0]->IndexBuffer.syncData();


	STransformation3 Transform;
	Transform.setScale(SVector3f(3.f));
	
	if (ShowVolume == 1)
	{
		glEnable(GL_CULL_FACE);

		glCullFace(GL_FRONT);
		{
			CShaderContext Context(* CShaderLoader::loadShader("Simple"));
			Context.bindBufferObject("aColor", VolumeCube->MeshBuffers[0]->ColorBuffer.getHandle(), 3);
			Context.bindBufferObject("aPosition", VolumeCube->MeshBuffers[0]->PositionBuffer.getHandle(), 3);

			Context.uniform("uModelMatrix", Transform.getGLMMat4());
			Context.uniform("uProjMatrix", SceneManager.getActiveCamera()->getProjectionMatrix());
			Context.uniform("uViewMatrix", SceneManager.getActiveCamera()->getViewMatrix());
			Context.bindIndexBufferObject(VolumeCube->MeshBuffers[0]->IndexBuffer.getHandle());

			VolumeTarget->bind();
			glDrawElements(GL_TRIANGLES, VolumeCube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}


		glCullFace(GL_BACK);
		{
			CShaderContext Context(* CShaderLoader::loadShader("Volume"));
			Context.bindBufferObject("aColor", VolumeCube->MeshBuffers[0]->ColorBuffer.getHandle(), 3);
			Context.bindBufferObject("aPosition", VolumeCube->MeshBuffers[0]->PositionBuffer.getHandle(), 3);

			Context.uniform("uModelMatrix", Transform.getGLMMat4());
			Context.uniform("uProjMatrix", SceneManager.getActiveCamera()->getProjectionMatrix());
			Context.uniform("uViewMatrix", SceneManager.getActiveCamera()->getViewMatrix());
			Context.uniform("uAlphaIntensity", Control.AlphaIntensity);

			Context.bindTexture("uBackPosition", VolumeBuffer->getTextureHandle());
			glEnable(GL_TEXTURE_3D);
			glActiveTexture(GL_TEXTURE0 + 1); // Select Active Texture Slot
			glBindTexture(GL_TEXTURE_3D, VolumeHandle); // Bind Texture Handle
			Context.uniform("uVolumeData", 1);

			Context.bindIndexBufferObject(VolumeCube->MeshBuffers[0]->IndexBuffer.getHandle());

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDrawElements(GL_TRIANGLES, VolumeCube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);

			
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_3D, 0);

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_3D);
		}

		glDisable(GL_CULL_FACE);
	}
	else if (ShowVolume == 2)
	{
		glEnable(GL_CULL_FACE);

		glCullFace(GL_FRONT);
		{
			CShader * Shader = CShaderLoader::loadShader("Volume2");

			if (Shader)
			{
				CShaderContext Context(* Shader);
				Context.bindBufferObject("aColor", VolumeCube->MeshBuffers[0]->ColorBuffer.getHandle(), 3);
				Context.bindBufferObject("aPosition", VolumeCube->MeshBuffers[0]->PositionBuffer.getHandle(), 3);

				Context.uniform("uModelMatrix", Transform.getGLMMat4());
				Context.uniform("uProjMatrix", SceneManager.getActiveCamera()->getProjectionMatrix());
				Context.uniform("uViewMatrix", SceneManager.getActiveCamera()->getViewMatrix());

				glEnable(GL_TEXTURE_3D);
				glActiveTexture(GL_TEXTURE0 + 0); // Select Active Texture Slot
				glBindTexture(GL_TEXTURE_3D, VolumeHandle); // Bind Texture Handle
				Context.uniform("uVolumeData", 0);

				Context.uniform("uAlphaIntensity", Control.AlphaIntensity);
				Context.uniform("uHighlightMode", Control.Mode);
				Context.uniform("uSliceAxis", Control.SliceAxis);
				Context.uniform("uLocalRange", Control.LocalRange);
				Context.uniform("uMinimumAlpha", Control.MinimumAlpha);
				Context.uniform("uEmphasisLocation", Control.EmphasisLocation);

				Context.uniform("uCameraPosition", SceneManager.getActiveCamera()->getPosition());

				Context.bindIndexBufferObject(VolumeCube->MeshBuffers[0]->IndexBuffer.getHandle());
			
				glEnable(GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDrawElements(GL_TRIANGLES, VolumeCube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);

			
				glBindTexture(GL_TEXTURE_3D, 0);
				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_3D);
			}
		}

		glDisable(GL_CULL_FACE);
	}

	return true;
}
