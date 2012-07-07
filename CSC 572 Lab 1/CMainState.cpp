#include "CMainState.h"

#include <Gwen/Gwen.h>
#include <Gwen/Renderers/SFML.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Controls.h>
#include <Gwen/Controls/VerticalSlider.h>
#include <Gwen/Controls/ComboBox.h>

#include "CGwenEventForwarder.h"
#include <CApplication.h>

#include "CVolumeControlsHandler.h"


CMainState::CMainState()
	: Camera(0), Tyra(0), Scale(1), Mode(0), BindLightPosition(LightPosition),
	ShowVolume(0), ShowGUI(true), ConsoleAccumulator(0.f), Slider(0.f)
{
	DataParser[0] = DataParser[1] = DataParser[2] = 0;
}

Gwen::Font * LoadFont(Gwen::UnicodeString const & File, float const Size)
{
	Gwen::Font * Font = new Gwen::Font();
	Font->facename = File;
	Font->size = Size;

	return Font;
}

void CMainState::begin()
{
	init();
	initScene();

	loadData();

	// setup GWEN
	Gwen::Renderer::SFML * pRenderer = new Gwen::Renderer::SFML(* CApplication::get().App);

	Gwen::Skin::TexturedBase * skin = new Gwen::Skin::TexturedBase(/*pRenderer*/);
	skin->SetRender(pRenderer);
	skin->Init("DefaultSkin.png");
	skin->SetDefaultFont(L"OpenSans.ttf", 14.f);
	Gwen::Font * LargeFont = LoadFont(L"OpenSans.ttf", 32.f);
	Gwen::Font * MediumFont = LoadFont(L"OpenSans.ttf", 24.f);
	Gwen::Font * RegularFont = LoadFont(L"OpenSans.ttf", 14.f);

	pCanvas = new Gwen::Controls::Canvas(skin);
	pCanvas->SetSize(Application.getWindowSize().X, Application.getWindowSize().Y);

	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(pCanvas);
	BigLabel->SetFont(LargeFont);
	BigLabel->SetText(Gwen::UnicodeString(L"Dataset: ") + Gwen::UnicodeString());
	BigLabel->SetBounds(0, 0, 600, 300);
	BigLabel->SetTextColor(Gwen::Color(255, 255, 255, 215));


	Gwen::Controls::Button * pButton = new Gwen::Controls::Button(pCanvas);
	pButton->SetBounds(1300, 50 + 120 + 10, 200, 25);
	pButton->SetText("Reset Volume Range");

	Gwen::Controls::Button * pButton2 = new Gwen::Controls::Button(pCanvas);
	pButton2->SetBounds(1300, 50 + 120 + 10 + 35, 200, 25);
	pButton2->SetText("Reset Alpha Intensity");	

	Gwen::Controls::Button * pButtonX = new Gwen::Controls::Button(pCanvas);
	pButtonX->SetBounds(1300, 50 + 120 + 10 + 45 + 25 + 35, 40, 25);
	pButtonX->SetText("X");

	Gwen::Controls::Button * pButtonY = new Gwen::Controls::Button(pCanvas);
	pButtonY->SetBounds(1350, 50 + 120 + 10 + 45 + 25 + 35, 40, 25);
	pButtonY->SetText("Y");

	Gwen::Controls::Button * pButtonZ = new Gwen::Controls::Button(pCanvas);
	pButtonZ->SetBounds(1400, 50 + 120 + 10 + 45 + 25 + 35, 40, 25);
	pButtonZ->SetText("Z");

	EmphasisSlider = new Gwen::Controls::VerticalSlider(pCanvas);
	EmphasisSlider->SetBounds(1300, 10, 40, 160);
	EmphasisSlider->SetRange(0.f, 1.f);

	Gwen::Controls::VerticalSlider * IntensitySlider = new Gwen::Controls::VerticalSlider(pCanvas);
	IntensitySlider->SetBounds(1350, 10, 40, 160);
	IntensitySlider->SetRange(10.f, 0.5f);

	Gwen::Controls::VerticalSlider * LocalRangeSlider = new Gwen::Controls::VerticalSlider(pCanvas);
	LocalRangeSlider->SetBounds(1400, 10, 40, 160);
	LocalRangeSlider->SetRange(0.05f, 0.5f);

	Gwen::Controls::VerticalSlider * MinimumAlphaSlider = new Gwen::Controls::VerticalSlider(pCanvas);
	MinimumAlphaSlider->SetBounds(1450, 10, 40, 160);
	MinimumAlphaSlider->SetRange(0.0f, 0.5f);

	Gwen::Controls::ComboBox * VolumeMode = new Gwen::Controls::ComboBox(pCanvas);
	VolumeMode->SetBounds(1300, 50 + 120 + 45 + 35, 200, 25);
	VolumeMode->AddItem(L"Full Volume");
	VolumeMode->AddItem(L"Plane Slices");
	VolumeMode->AddItem(L"Isosurface");

	CVolumeControlsHandler * Handler = new CVolumeControlsHandler(/*DataParser*/);
	EmphasisSlider->onValueChanged.Add(Handler, & CVolumeControlsHandler::OnEmphasisSlider);
	IntensitySlider->onValueChanged.Add(Handler, & CVolumeControlsHandler::OnIntensitySlider);
	MinimumAlphaSlider->onValueChanged.Add(Handler, & CVolumeControlsHandler::OnMinimumAlphaSlider);
	LocalRangeSlider->onValueChanged.Add(Handler, & CVolumeControlsHandler::OnLocalRangeSlider);
	pButton->onPress.Add(Handler, & CVolumeControlsHandler::OnResetVolume);
	pButton2->onPress.Add(Handler, & CVolumeControlsHandler::OnResetAlpha);
	pButtonX->onPress.Add(Handler, & CVolumeControlsHandler::OnSetXAxis);
	pButtonY->onPress.Add(Handler, & CVolumeControlsHandler::OnSetYAxis);
	pButtonZ->onPress.Add(Handler, & CVolumeControlsHandler::OnSetZAxis);
	VolumeMode->onSelection.Add(Handler, & CVolumeControlsHandler::OnVolumeMode);

	for (int i = 0; i < ConsoleSize; ++ i)
	{
		ConsoleMessages[i] = new Gwen::Controls::Label(pCanvas);
		ConsoleMessages[i]->SetPos(20, 900 - 50 - 25 * i);
		ConsoleMessages[i]->SetSize(1500, 30);
		ConsoleMessages[i]->SetShouldDrawBackground(true);
	}

	addConsoleMessage("GUI Initialized.");
	addConsoleMessage("Starting program...", Gwen::Colors::Red);

	CGwenEventForwarder * Forwarder = new CGwenEventForwarder(pCanvas);

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
	VolumeTarget->attach(VolumeBuffer, GL_COLOR_ATTACHMENT0);//*/

	addConsoleMessage("Volume mesh created.", Gwen::Color(0, 255, 0));

	Timer = 0.f;
}

void CMainState::OnRenderStart(float const Elapsed)
{
	Camera->update(Elapsed);

	Timer += Elapsed * 0.16f;

	float const Distance = 4.f;
	OrbitCamera->setPosition(SVector3f(sin(Timer)*Distance, 2.3f, cos(Timer)*Distance));
	OrbitCamera->setLookAtTarget(SVector3f());

	Tyra->setTranslation(Translation);
    Tyra->setScale(Scale);
    Tyra->setRotation(Rotation);

	LightPosition = SceneManager.getActiveCamera()->getPosition() + SVector3f(0, 0, 0);

	LightObject->setTranslation(LightPosition);

	/*Slider += Elapsed * 0.5f;
	COxygenLocalizedColorMapper l;
	l.EmphasisLocation = (sin(Slider) + 1.f) / 2.f;
	DataParser->createVolumeFromGridValues(& l);*/
	

	SceneManager.drawAll();
	SceneManager.endDraw();


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
			Context.uniform("uAlphaIntensity", Volume.AlphaIntensity);

			Context.bindTexture("uBackPosition", VolumeBuffer->getTextureHandle());
			glEnable(GL_TEXTURE_3D);
			glActiveTexture(GL_TEXTURE0 + 1); // Select Active Texture Slot
			glBindTexture(GL_TEXTURE_3D, DataParser[2]->VolumeHandle); // Bind Texture Handle
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
				glBindTexture(GL_TEXTURE_3D, DataParser[2]->VolumeHandle); // Bind Texture Handle
				Context.uniform("uVolumeData", 0);

				Context.uniform("uAlphaIntensity", Volume.AlphaIntensity);
				Context.uniform("uHighlightMode", Volume.Mode);
				Context.uniform("uSliceAxis", Volume.SliceAxis);
				Context.uniform("uLocalRange", Volume.LocalRange);
				Context.uniform("uMinimumAlpha", Volume.MinimumAlpha);
				Context.uniform("uEmphasisLocation", Volume.EmphasisLocation);

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

	
	glBindTexture(GL_TEXTURE_2D, 0);
	

	if (ShowGUI)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glClear(GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		int left = 0, top = 0;
		int right = 1600, bottom = 900;
		glOrtho( left, right, bottom, top, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(0, 0, right - left, bottom - top);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		static float const AlphaTick = 0.1f;
		ConsoleAccumulator += Elapsed;

		while (ConsoleAccumulator > AlphaTick)
		{
			for (int i = 0; i < ConsoleSize; ++ i)
			{
				static int const Decrement = 3;
				Gwen::Color & c = ConsoleMessageColors[i];//->GetTextColor();

				if (c.a >= Decrement)
					c.a -= Decrement;
				else
					c.a = 0;
				
				ConsoleMessages[i]->SetTextColor(c);
			}

			ConsoleAccumulator -= AlphaTick;
		}


		pCanvas->RenderCanvas();
	}

	CApplication::get().swapBuffers();

	//printOpenGLErrors("post swap");
	//Sleep(3000);
}

void CMainState::addConsoleMessage(std::string const & Message, Gwen::Color const & Color)
{
	for (int i = ConsoleSize - 1; i > 0; -- i)
	{
		ConsoleMessages[i]->SetText(ConsoleMessages[i-1]->GetText());
		ConsoleMessageColors[i] = ConsoleMessageColors[i-1];
		ConsoleMessages[i]->SetTextColor(ConsoleMessageColors[i]);
	}

	ConsoleMessages[0]->SetText(Message);
	ConsoleMessageColors[0] = Color;
	ConsoleMessages[0]->SetTextColor(Color);
}
