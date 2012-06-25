#include "CMainState.h"

#include <Gwen/Gwen.h>
#include <Gwen/Renderers/OpenGL.h>
#include <Gwen/Renderers/OpenGL_DebugFont.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Controls.h>

CMainState::CMainState()
	: Camera(0), Tyra(0), Scale(1), Mode(3), BindLightPosition(LightPosition),
	ShowVolume(0)
{}

void CMainState::begin()
{
	init();
	initScene();

	loadData();

	// setup GWEN
	Gwen::Renderer::OpenGL * pRenderer = new Gwen::Renderer::OpenGL_DebugFont();

	Gwen::Skin::Simple * skin = new Gwen::Skin::Simple();
	skin->SetRender(pRenderer);
	/*Gwen::Skin::TexturedBase * skin = new Gwen::Skin::TexturedBase();
	skin->SetRender(pRenderer);
	skin->Init("DefaultSkin.png");*/

	pCanvas = new Gwen::Controls::Canvas(skin);
	pCanvas->SetSize(500, 500);
	pCanvas->SetDrawBackground(true);
	pCanvas->SetBackgroundColor(Gwen::Color(240, 120, 120, 255));

	Gwen::Controls::Button * pButton = new Gwen::Controls::Button(pCanvas);
	pButton->SetBounds(0, 0, 200, 100);
	pButton->SetText("My First Button");

	// Setup volume cube
	VolumeCube = new CMesh();
    CMesh::SMeshBuffer * Mesh = new CMesh::SMeshBuffer();

    Mesh->Vertices.resize(24);
    Mesh->Vertices[0].Position = SVector3f(-0.5, -0.5, -0.5);
    Mesh->Vertices[1].Position = SVector3f(-0.5,  0.5, -0.5);
    Mesh->Vertices[2].Position = SVector3f( 0.5,  0.5, -0.5);
    Mesh->Vertices[3].Position = SVector3f( 0.5, -0.5, -0.5);
	Mesh->Vertices[0].Color = SColor(0, 0, 0);
	Mesh->Vertices[1].Color = SColor(0, 1, 0);
	Mesh->Vertices[2].Color = SColor(1, 1, 0);
	Mesh->Vertices[3].Color = SColor(1, 0, 0);

    Mesh->Vertices[4].Position = SVector3f( 0.5, -0.5, -0.5);
    Mesh->Vertices[5].Position = SVector3f( 0.5,  0.5, -0.5);
    Mesh->Vertices[6].Position = SVector3f( 0.5,  0.5,  0.5);
    Mesh->Vertices[7].Position = SVector3f( 0.5, -0.5,  0.5);
	Mesh->Vertices[4].Color = SColor(1, 0, 0);
	Mesh->Vertices[5].Color = SColor(1, 1, 0);
	Mesh->Vertices[6].Color = SColor(1, 1, 1);
	Mesh->Vertices[7].Color = SColor(1, 0, 1);

    Mesh->Vertices[8].Position = SVector3f(  0.5, -0.5,  0.5);
    Mesh->Vertices[9].Position = SVector3f(  0.5,  0.5,  0.5);
    Mesh->Vertices[10].Position = SVector3f(-0.5,  0.5,  0.5);
    Mesh->Vertices[11].Position = SVector3f(-0.5, -0.5,  0.5);
	Mesh->Vertices[8].Color = SColor(1, 0, 1);
	Mesh->Vertices[9].Color = SColor(1, 1, 1);
	Mesh->Vertices[10].Color = SColor(0, 1, 1);
	Mesh->Vertices[11].Color = SColor(0, 0, 1);

    Mesh->Vertices[12].Position = SVector3f(-0.5, -0.5,  0.5);
    Mesh->Vertices[13].Position = SVector3f(-0.5,  0.5,  0.5);
    Mesh->Vertices[14].Position = SVector3f(-0.5,  0.5, -0.5);
    Mesh->Vertices[15].Position = SVector3f(-0.5, -0.5, -0.5);
	Mesh->Vertices[12].Color = SColor(0, 0, 1);
	Mesh->Vertices[13].Color = SColor(0, 1, 1);
	Mesh->Vertices[14].Color = SColor(0, 1, 0);
	Mesh->Vertices[15].Color = SColor(0, 0, 0);

    Mesh->Vertices[16].Position = SVector3f(-0.5,  0.5, -0.5);
    Mesh->Vertices[17].Position = SVector3f(-0.5,  0.5,  0.5);
    Mesh->Vertices[18].Position = SVector3f( 0.5,  0.5,  0.5);
    Mesh->Vertices[19].Position = SVector3f( 0.5,  0.5, -0.5);
	Mesh->Vertices[16].Color = SColor(0, 1, 0);
	Mesh->Vertices[17].Color = SColor(0, 1, 1);
	Mesh->Vertices[18].Color = SColor(1, 1, 1);
	Mesh->Vertices[19].Color = SColor(1, 1, 0);

    Mesh->Vertices[20].Position = SVector3f( 0.5, -0.5, -0.5);
    Mesh->Vertices[21].Position = SVector3f( 0.5, -0.5,  0.5);
    Mesh->Vertices[22].Position = SVector3f(-0.5, -0.5,  0.5);
    Mesh->Vertices[23].Position = SVector3f(-0.5, -0.5, -0.5);
	Mesh->Vertices[20].Color = SColor(1, 0, 0);
	Mesh->Vertices[21].Color = SColor(1, 0, 1);
	Mesh->Vertices[22].Color = SColor(0, 0, 1);
	Mesh->Vertices[23].Color = SColor(0, 0, 0);

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

void CMainState::OnRenderStart(float const Elapsed)
{
	Camera->update(Elapsed);

	Tyra->setTranslation(Translation);
    Tyra->setScale(Scale);
    Tyra->setRotation(Rotation);

	LightPosition = Camera->getPosition() + SVector3f(0, 0, 0);

	LightObject->setTranslation(LightPosition);
	

	SceneManager.drawAll();
	SceneManager.endDraw();


	// Check data syncing
	if (VolumeCube->MeshBuffers[0]->PositionBuffer.isDirty())
		VolumeCube->MeshBuffers[0]->PositionBuffer.syncData();
	if (VolumeCube->MeshBuffers[0]->ColorBuffer.isDirty())
		VolumeCube->MeshBuffers[0]->ColorBuffer.syncData();
	if (VolumeCube->MeshBuffers[0]->IndexBuffer.isDirty())
		VolumeCube->MeshBuffers[0]->IndexBuffer.syncData();

	
	
	if (ShowVolume == 1)
	{
		glEnable(GL_CULL_FACE);

		glCullFace(GL_FRONT);
		{
			CShaderContext Context(* CShaderLoader::loadShader("Simple"));
			Context.bindBufferObject("aColor", VolumeCube->MeshBuffers[0]->ColorBuffer.getHandle(), 3);
			Context.bindBufferObject("aPosition", VolumeCube->MeshBuffers[0]->PositionBuffer.getHandle(), 3);

			Context.uniform("uModelMatrix", STransformation3().getGLMMat4());
			Context.uniform("uProjMatrix", Camera->getProjectionMatrix());
			Context.uniform("uViewMatrix", Camera->getViewMatrix());
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

			Context.uniform("uModelMatrix", STransformation3().getGLMMat4());
			Context.uniform("uProjMatrix", Camera->getProjectionMatrix());
			Context.uniform("uViewMatrix", Camera->getViewMatrix());

			Context.bindTexture("uBackPosition", VolumeBuffer->getTextureHandle());
			glEnable(GL_TEXTURE_3D);
			glActiveTexture(GL_TEXTURE0 + 1); // Select Active Texture Slot
			glBindTexture(GL_TEXTURE_3D, DataSet.VolumeHandle); // Bind Texture Handle
			Context.uniform("uVolumeData", 1);

			Context.bindIndexBufferObject(VolumeCube->MeshBuffers[0]->IndexBuffer.getHandle());

			glEnable(GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDrawElements(GL_TRIANGLES, VolumeCube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);
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
			CShaderContext Context(* CShaderLoader::loadShader("Volume2"));
			Context.bindBufferObject("aColor", VolumeCube->MeshBuffers[0]->ColorBuffer.getHandle(), 3);
			Context.bindBufferObject("aPosition", VolumeCube->MeshBuffers[0]->PositionBuffer.getHandle(), 3);

			Context.uniform("uModelMatrix", STransformation3().getGLMMat4());
			Context.uniform("uProjMatrix", Camera->getProjectionMatrix());
			Context.uniform("uViewMatrix", Camera->getViewMatrix());

			glEnable(GL_TEXTURE_3D);
			glActiveTexture(GL_TEXTURE0 + 0); // Select Active Texture Slot
			glBindTexture(GL_TEXTURE_3D, DataSet.VolumeHandle); // Bind Texture Handle
			Context.uniform("uVolumeData", 0);

			Context.uniform("uCameraPosition", Camera->getPosition());

			Context.bindIndexBufferObject(VolumeCube->MeshBuffers[0]->IndexBuffer.getHandle());
			
			glEnable(GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDrawElements(GL_TRIANGLES, VolumeCube->MeshBuffers[0]->IndexBuffer.getElements().size(), GL_UNSIGNED_SHORT, 0);
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_3D);
		}

		glDisable(GL_CULL_FACE);
	}
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	int left = 0, top = 0;
	int right = 1600, bottom = 900;
	glOrtho( left, right, bottom, top, -1.0, 1.0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glViewport(0, 0, right - left, bottom - top);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//pCanvas->RenderCanvas();

    SDL_GL_SwapBuffers();
}
