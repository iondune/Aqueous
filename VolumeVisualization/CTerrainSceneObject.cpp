#include "CTerrainSceneObject.h"

#include "CProgramContext.h"


CTerrainSceneObject::SLayer::SLayer(int const i)
{
	// Scaling Parameters
	Level = i;
	ScaleFactor = 1 << i;

	// Create Heightmap Texture
	STextureCreationFlags Flags;
	Flags.Filter = GL_LINEAR;
	Flags.MipMaps = false;
	ColorMap = new CTexture(CImageLoader::loadImage("../TerrainColorImageSquare.bmp"));
	Flags.PixelInternalFormat = GL_R32F;
	//Flags.Wrap = GL_CLAMP;

	HeightMap = new CTexture(CImageLoader::loadImage("../TerrainHeightImageSquare.bmp"));

	// Determine starting ClipRegion
	SVector2i const ClipPos = SVector2i() - SVector2i(Size / 2);
	ClipRegion = SRect2i(ClipPos, SVector2i(Size));
}

int CTerrainSceneObject::SLayer::sendSample(int const x1, int const y1, int const x2, int const y2, SVector2i const & NewClipPos)
{

	static const auto noise = [](int const x, int const y) -> float
	{
		return (float) 0.f;
	};

	static const auto color = [](int const x, int const y) -> SColorAf
	{				
		return SColorAf();
	};

	int const Size = (x2 - x1) * (y2 - y1);

	if (x2 < x1)
	{
		std::cerr << "Invalid sample size parameters! x2=" << x2 << " x1=" << x1 << ". Aborting!" << std::endl;
		return -1;
	}

	if (y2 < y1)
	{
		std::cerr << "Invalid sample size parameters! y2=" << y1 << " y1=" << y1 << ". Aborting!" << std::endl;
		return -1;
	}

	if ((x2 - x1) > HeightmapSize || (y2 - y1) > HeightmapSize || x2 > HeightmapSize || y2 > HeightmapSize)
	{
		std::cerr << "Sample size parameters too large! x2=" << x2 << " x1=" << x1 << " y2=" << y2 << " y1=" << y1 << ".  Aborting!" << std::endl;
		return -1;
	}

	float * Data = new float[Size];
	float * Color = new float[Size * 3];

	bool Succeeded = true;

	for (int y = y1; y < y2; ++ y)
	{
		for (int x = x1; x < x2; ++ x)
		{
			int const Index = (x - x1) + (y - y1) * (x2 - x1);
			if (Index >= Size || Index < 0)
			{
				std::cerr << "Out of bounds index!" << std::endl;
				Succeeded = false;
			}
			else
			{
				int DataX = x;
				if (DataX < DataOffset.X)
					DataX += (HeightmapSize);
				DataX -= DataOffset.X;
				DataX += NewClipPos.X;
				int DataY = y;
				if (DataY < DataOffset.Y)
					DataY += (HeightmapSize);
				DataY -= DataOffset.Y;
				DataY += NewClipPos.Y;

				Data[Index] = noise(DataX * ScaleFactor, DataY * ScaleFactor);

				HostHeightMap[x][y] = Data[Index];

				SColorf colorSample = color(DataX * ScaleFactor, DataY * ScaleFactor);
				Color[Index * 3 + 0] = colorSample.Red;
				Color[Index * 3 + 1] = colorSample.Green;
				Color[Index * 3 + 2] = colorSample.Blue;
			}
		}
	}

			
	glBindTexture(GL_TEXTURE_2D, HeightMap->getTextureHandle());
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		x1, y1,
		x2 - x1, y2 - y1,
		GL_RED,
		GL_FLOAT,
		Data);

	glBindTexture(GL_TEXTURE_2D, ColorMap->getTextureHandle());
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		x1, y1,
		x2 - x1, y2 - y1,
		GL_RGB,
		GL_FLOAT,
		Color);

	delete [] Data;
	delete [] Color;

	if (Succeeded)
		return (x2 - x1) * (y2 - y1);

	return -1;
}



CTerrainSceneObject::CTerrainSceneObject()
	: Application(CApplication::get()), SceneManager(CApplication::get().getSceneManager()), DrawLevel(0), DoCameraUpdate(true), DebugHeight(false)
{
	setCullingEnabled(false);

	Shader = CProgramContext::get().Shaders.Terrain;

		
	for (int y = 0; y < HeightmapSize; ++ y)
	{
		for (int x = 0; x < HeightmapSize; ++ x)
		{
			VertexData.push_back(x * 1.f);
			VertexData.push_back(y * 1.f);
		}
	}


	VertexData.syncData();

	for (int i = 0; i < LayerCount; ++ i)
		Layers.push_back(new SLayer(i));
}

float const CTerrainSceneObject::getTerrainHeight(SVector2f const & Position)
{
	SVector2i Pos = SVector2i((int) Position.X, (int) Position.Y);

	if (Layers[0]->ClipRegion.isPointInside(SVector2<int>(Pos)))
	{
		SVector2<int> Index = SVector2<int>(Pos) - Layers[0]->ClipRegion.Position;
		Index += SVector2<int>(Layers[0]->DataOffset);
		float Value = Layers[0]->HostHeightMap[Index.X % HeightmapSize][Index.Y % HeightmapSize];


		return Value;
	}
		
	return 0.f;
}

bool CTerrainSceneObject::draw(IScene const * const Scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled)
{
	if (! ISceneObject::draw(Scene, Pass, CullingEnabled))
		return false;

	// Setup shared shader parameters
	CShaderContext Context(* Shader);
	Context.uniform("uViewMatrix", SceneManager.getActiveCamera()->getViewMatrix());
	Context.uniform("uProjMatrix", SceneManager.getActiveCamera()->getProjectionMatrix());
	Context.uniform("uLayerWidth", (float) Size);
	Context.uniform("uLightPosition", CProgramContext::get().Scene.LightPosition);
	int DebugHeightUniform = DebugHeight ? 1 : 0;
	Context.uniform("uDebugHeight", DebugHeightUniform);
	Context.bindBufferObject("aPosition", VertexData.getHandle(), 2);

	// Determine camera movement logistics for height culling
	if (DoCameraUpdate)
	{
		for (auto it = Layers.begin(); it != Layers.end(); ++ it)
			(* it)->Active = true;
	}
		
	// Determine camera movement logistics for tracking
	static SVector2i CameraPos;

	if (DoCameraUpdate)
		CameraPos = SVector2i(
		(int) std::floor(SceneManager.getActiveCamera()->getPosition().X), 
		(int) std::floor(SceneManager.getActiveCamera()->getPosition().Z));
	CameraPos = vec2i();


	static const auto even = [](int const i) -> int
	{
		return i % 2 ? i - 1 : i;
	};

	static const auto wrap = [](SVector2i const & p) -> SVector2i
	{
		return SVector2i(p.X % HeightmapSize, p.Y % HeightmapSize);
	};

	static const auto sq = [](int const i) -> int
	{
		return i * i;
	};


	///////////////////////////////////////////////////////////
	// Initial Pass - Calculate regions and generate samples //
	///////////////////////////////////////////////////////////

	int Budget = 1000;

	for (int i = LayerCount - 1; i >= 0; -- i)
	{
		if (Budget < 0)
			break;

		SLayer * Layer = Layers[i];

		//////////////////////////////////////////////////////
		// Calculate new Active Region from Camera Position //
		//////////////////////////////////////////////////////

		SVector2i CenterPos = CameraPos / Layer->ScaleFactor;
		// only allow even coordinates so that inner layers occupy the same grid as outer ones
		CenterPos.X = even(CenterPos.X);
		CenterPos.Y = even(CenterPos.Y);

		// Set desired active region centered on camera
		Layer->ActiveRegion = SRect2i(CenterPos - SVector2i(Size / 2), SVector2i(Size - 1));

		// Attempt to calculate budgeted offset move
		SVector2i const OldClipPos = SVector2i(Layer->ClipRegion.Position.X, Layer->ClipRegion.Position.Y);
		SVector2i DataOffsetMove = CenterPos - SVector2i(Size / 2) - OldClipPos;

		for (int t = 0; t < 5; ++ t)
		{
			int Heuristic = Size * (DataOffsetMove.X + DataOffsetMove.Y);
			if (Budget - Heuristic < 0)
				DataOffsetMove /= 2;
		}

		SVector2i const NewClipPos = DataOffsetMove + OldClipPos;
		Layer->ClipRegion = SRect2i(NewClipPos, SVector2i(Size));
		Layer->ActiveRegion.clipTo(Layer->ClipRegion);

		// If no affordable move, continue on
		if (DataOffsetMove.X == 0 && DataOffsetMove.Y == 0)
			continue;

		//std::cout << "Data movement (" << i << "): " << DataOffsetMove.X << ", " << DataOffsetMove.Y << " ";
			
		////////////////////////////////////////////////////////////
		// Determine and generate new sample data for Clip Region //
		////////////////////////////////////////////////////////////

		int SamplesSent = 0, TotalSamples = 0;

		if (abs(DataOffsetMove.X) >= HeightmapSize || abs(DataOffsetMove.Y) >= HeightmapSize)
		{
			// None of the old data is kept, so just reload entire heightmap to make things simpler
			Layer->DataOffset.set(0);

			if ((SamplesSent = Layer->sendSample(0, 0, HeightmapSize, HeightmapSize, NewClipPos)) == -1)
				std::cout << "sendSample failure in full update" << std::endl;

			std::cout << "Full update to 0, 0." << std::endl;
		}
		else
		{
			// Reload new data for toroidal access
			SVector2i OriginalDataOffset = Layer->DataOffset;
			Layer->DataOffset += DataOffsetMove;
			//std::cout << "to " << Layer->DataOffset.X << ", " << Layer->DataOffset.Y << "." << std::endl;

			SRect2i Bounds;

			if (Layer->DataOffset.X >= HeightmapSize && Layer->DataOffset.Y >= HeightmapSize || Layer->DataOffset.X < 0 && Layer->DataOffset.Y < 0)
			{
				if (Layer->DataOffset.X < 0)
					while (Layer->DataOffset.X < 0)
						Layer->DataOffset.X += HeightmapSize;
				else
					Layer->DataOffset.X %= HeightmapSize;
					
				if (Layer->DataOffset.Y < 0)
					while (Layer->DataOffset.Y < 0)
						Layer->DataOffset.Y += HeightmapSize;
				else
					Layer->DataOffset.Y %= HeightmapSize;

				Bounds.bounds(OriginalDataOffset, Layer->DataOffset);

				// Double wrap around, 4 quad updates around edges
				if ((SamplesSent = Layer->sendSample(0, 0, Bounds.otherCorner().X, Bounds.Position.Y, NewClipPos)) == -1)
					std::cout << "sendSample failure in double wrap update, pass 1" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(Bounds.otherCorner().X, 0, HeightmapSize, Bounds.otherCorner().Y, NewClipPos)) == -1)
					std::cout << "sendSample failure in double wrap update, pass 2" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(Bounds.Position.X, Bounds.otherCorner().Y, HeightmapSize, HeightmapSize, NewClipPos)) == -1)
					std::cout << "sendSample failure in double wrap update, pass 3" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(0, Bounds.Position.Y, Bounds.Position.X, HeightmapSize, NewClipPos)) == -1)
					std::cout << "sendSample failure in double wrap update, pass 4" << std::endl;
				else
					TotalSamples += SamplesSent;
					
				//std::cout << "Double wrap-around update." << std::endl;
			}
			else if (Layer->DataOffset.X >= HeightmapSize || Layer->DataOffset.X < 0)
			{
				if (Layer->DataOffset.X < 0)
					while (Layer->DataOffset.X < 0)
						Layer->DataOffset.X += HeightmapSize;
				else
					Layer->DataOffset.X %= HeightmapSize;

				Bounds.bounds(OriginalDataOffset, Layer->DataOffset);

				// Wrap around in X, 3 quads on horizontal edges and across
				if ((SamplesSent = Layer->sendSample(0, 0, Bounds.Position.X, HeightmapSize, NewClipPos)) == -1)
					std::cout << "sendSample failure in x wrap update, pass 1" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(Bounds.Position.X, Bounds.Position.Y, Bounds.otherCorner().X, Bounds.otherCorner().Y, NewClipPos)) == -1)
					std::cout << "sendSample failure in x wrap update, pass 2" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(Bounds.otherCorner().X, 0, HeightmapSize, HeightmapSize, NewClipPos)) == -1)
					std::cout << "sendSample failure in x wrap update, pass 3" << std::endl;
				else
					TotalSamples += SamplesSent;
					
				//std::cout << "X-wrap update." << std::endl;
			}
			else if (Layer->DataOffset.Y >= HeightmapSize || Layer->DataOffset.Y < 0)
			{
				if (Layer->DataOffset.Y < 0)
					while (Layer->DataOffset.Y < 0)
						Layer->DataOffset.Y += HeightmapSize;
				else
					Layer->DataOffset.Y %= HeightmapSize;
					
				Bounds.bounds(OriginalDataOffset, Layer->DataOffset);

				// Wrap around in Y, 3 quads on vertical edges and across
				if ((SamplesSent = Layer->sendSample(0, 0, HeightmapSize, Bounds.Position.Y, NewClipPos)) == -1)
					std::cout << "sendSample failure in y wrap update, pass 1" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(Bounds.Position.X, Bounds.Position.Y, Bounds.otherCorner().X, Bounds.otherCorner().Y, NewClipPos)) == -1)
					std::cout << "sendSample failure in y wrap update, pass 2" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(0, Bounds.otherCorner().Y, HeightmapSize, HeightmapSize, NewClipPos)) == -1)
					std::cout << "sendSample failure in y wrap update, pass 3" << std::endl;
				else
					TotalSamples += SamplesSent;

				//std::cout << "Y-wrap update." << std::endl;
			}
			else
			{
				Bounds.bounds(OriginalDataOffset, Layer->DataOffset);

				// No wrap around, 3 quads to create a plus centered on Bounds
				if ((SamplesSent = Layer->sendSample(Bounds.Position.X, 0, Bounds.otherCorner().X, HeightmapSize, NewClipPos)) == -1)
					std::cout << "sendSample failure in no-wrap update, pass 1" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(0, Bounds.Position.Y, Bounds.Position.X, Bounds.otherCorner().Y, NewClipPos)) == -1)
					std::cout << "sendSample failure in no-wrap update, pass 2" << std::endl;
				else
					TotalSamples += SamplesSent;
				if ((SamplesSent = Layer->sendSample(Bounds.otherCorner().X, Bounds.Position.Y, HeightmapSize, Bounds.otherCorner().Y, NewClipPos)) == -1)
					std::cout << "sendSample failure in no-wrap update, pass 3" << std::endl;
				else
					TotalSamples += SamplesSent;

				//std::cout << "No-wrap update." << std::endl;
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		Budget -= TotalSamples;
	}


	///////////////////////////////////////////////
	// Second Pass - Generate Indices and Render //
	///////////////////////////////////////////////
	
	if (isDebugDataEnabled(EDebugData::Wireframe))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	for (auto it = Layers.begin(); it != Layers.end(); ++ it)
	{
		SLayer * Layer = * it;
		SLayer * Course = it + 1 == Layers.end() ? 0 : * (it + 1);

		if (! Layer->Active)
			continue;

		if (Layer->Level < DrawLevel)
			continue;

		int ScaleFactor = Layer->ScaleFactor;
		float Scale = ScaleFactor * 1.f;

		Layer->IndexBuffer.clear();

		for (int x = Layer->ActiveRegion.Position.X; x <= Layer->ActiveRegion.otherCorner().X; ++ x)
		{
			for (int y = Layer->ActiveRegion.Position.Y; y <= Layer->ActiveRegion.otherCorner().Y; ++ y)
			{
				if (Layer->Level && (* (it - 1))->Active)
				{
					SRect2i Bound = ((* (it - 1))->ActiveRegion);
					Bound.Position /= 2;
					Bound.Size /= 2;
					if (Bound.isPointInsideOrOn(SVector2i(x, y)) && Bound.Size.X && Bound.Size.Y)
						continue;
				}

				int X = x - Layer->ClipRegion.Position.X;
				int Y = y - Layer->ClipRegion.Position.Y;

				if (X >= HeightmapSize || Y >= HeightmapSize || X < 0 || Y < 0)
				{
					printf("Error: Out of bound index-buffer location\n");
					continue;
				}

				Layer->IndexBuffer.push_back(X + 0 + (Size + 1) * (Y + 0));
				Layer->IndexBuffer.push_back(X + 0 + (Size + 1) * (Y + 1));
				Layer->IndexBuffer.push_back(X + 1 + (Size + 1) * (Y + 1));

				Layer->IndexBuffer.push_back(X + 0 + (Size + 1) * (Y + 0));
				Layer->IndexBuffer.push_back(X + 1 + (Size + 1) * (Y + 1));
				Layer->IndexBuffer.push_back(X + 1 + (Size + 1) * (Y + 0));
			}
		}

		Layer->IndexBuffer.syncData();

			
		SVector2i const DataOffseti = Layer->DataOffset;
		SVector2f const DataOffsetf((float) DataOffseti.X, (float) DataOffseti.Y);

		SVector2f Translation = SVector2f((float) Layer->ClipRegion.Position.X, (float) Layer->ClipRegion.Position.Y) * (float) ScaleFactor;

		Layer->Transformation.setTranslation(SVector3f(Translation.X, 0.f, Translation.Y));
		Layer->Transformation.setScale(SVector3f(1.f * ScaleFactor, 1.f, 1.f * ScaleFactor));

		Context.uniform("uModelMatrix", Transformation.getGLMMat4() * (ModelMatrix = (* it)->Transformation.getGLMMat4()));
		Context.uniform("uDataOffset", DataOffsetf);

		if (Course)
		{
			Context.uniform("uUseCourse", 1);

			SVector2i CenterPos = CameraPos / Layer->ScaleFactor;
			CenterPos.X = even(CenterPos.X);
			CenterPos.Y = even(CenterPos.Y);

			SVector2i LowerCorner = CenterPos - SVector2i(Size / 2);

			SVector2<int> uActiveRegionLL = Layer->ActiveRegion.Position - Layer->ClipRegion.Position;
			SVector2<int> uActiveRegionUR = Layer->ActiveRegion.otherCorner() - Layer->ClipRegion.Position;

			if (uActiveRegionLL.X < 0)
			{
				uActiveRegionLL.X = 0;
				printf("Error: Negative coordinates in lower-left active region.\n");
			}

			if (uActiveRegionLL.Y < 0)
			{
				uActiveRegionLL.Y = 0;
				printf("Error: Negative coordinates in lower-left active region.\n");
			}


			if (uActiveRegionUR.Y > Layer->ClipRegion.Size.Y)
			{
				uActiveRegionUR.Y = Layer->ClipRegion.Size.Y;
				printf("Error: Positive coordinates in upper-right active region.\n");
			}

			if (uActiveRegionUR.X > Layer->ClipRegion.Size.X)
			{
				uActiveRegionUR.X = Layer->ClipRegion.Size.X;
				printf("Error: Positive coordinates in upper-right active region.\n");
			}


			Context.uniform("uActiveRegionLL", SVector2f(uActiveRegionLL));
			Context.uniform("uActiveRegionUR", SVector2f(uActiveRegionUR));
		}
		else
		{
			Context.uniform("uUseCourse", 0);
		}

		Context.bindTexture("uHeightMap", Layer->HeightMap->getTextureHandle());
		Context.bindTexture("uColorMap", Layer->ColorMap->getTextureHandle());

		Context.bindIndexBufferObject((* it)->IndexBuffer.getHandle());

		glDrawElements(GL_TRIANGLES, (* it)->IndexBuffer.getElements().size(), GL_UNSIGNED_INT, 0);
	}
	
	if (isDebugDataEnabled(EDebugData::Wireframe))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	return true;
}
