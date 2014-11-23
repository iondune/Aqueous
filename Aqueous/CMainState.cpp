
#include "CMainState.h"

#include "CTerrainNodeManager.h"
#include "CGlyphNodeManager.h"
#include "CSite.h"
#include "CSpectrumColorMapper.h"
#include "CGUIContext.h"

#include <ionScience.h>


CMainState::CMainState()
	: Scale(1), Mode(0), ShowDepth(false), ProjectionMode(0), GUIEnabled(true), OrbitCameraTimer(0), gifWriter()
{}

void CMainState::Begin()
{
	Context->GUIContext->SetupMainState();

	Context->Scene.Timer = 0.f;
	//Context->Scene.Glyphs->BuildLines();

	//ReflectionRenderPass = sharedNew<CReflectionRenderPass>(new CReflectionRenderPass);
	//SceneManager->getEffectManager()->RenderPasses.push_back(ReflectionRenderPass);

	CalculateDataAlignment();
	OrbitCameraTimer = 0;

	Font = IFont::init("Media/GUI/OpenSans.ttf", 18);
}

void CMainState::End()
{
	Context->GUIContext->Clear();
}

void CMainState::BeginGifDraw()
{
	SceneManager->GetScene()->SetActiveCamera(Context->Scene.OrbitCamera);
	gifWriter = new GifWriter(Context->Window->GetSize());
	OrbitCameraTimer = 0;
}

void CMainState::EndGifDraw()
{
	SceneManager->GetScene()->SetActiveCamera(Context->Scene.Camera);
	if (! gifWriter->Save("output.gif"))
	{
		std::cerr << "GIF writing failed" << std::endl;
		WaitForUser();
	}

	delete gifWriter;
	gifWriter = 0;
}

f64 GlobalMin, GlobalMax;

void CMainState::Update(f32 const Elapsed)
{
	CProgramContext::SScene & Scene = Context->Scene;

	Scene.CameraController->Update(Elapsed);

	f32 Radius = Scene.Camera->GetPosition().Length();
	if (Radius < 10.f)
	{
		Scene.Camera->SetNearPlane(0.001f);
		Scene.Camera->SetFarPlane(100.f);
	}
	else if (Radius < 50.f)
	{
		Scene.Camera->SetNearPlane(0.01f);
		Scene.Camera->SetFarPlane(150.f);
	}
	else if (Radius < 500.f)
	{
		Scene.Camera->SetNearPlane(10.f);
		Scene.Camera->SetFarPlane(1000.f);
	}
	else if (Radius < 5000.f)
	{
		Scene.Camera->SetNearPlane(100.f);
		Scene.Camera->SetFarPlane(10000.f);
	}
	else if (Radius < 15000.f)
	{
		Scene.Camera->SetNearPlane(4000.f);
		Scene.Camera->SetFarPlane(20000.f);
	}

	Scene.Timer += Elapsed * 0.16f;

	float const Distance = 2.5f;
	static f32 const Speed = 1.f;
	static f32 const Increment = 0.05f;
	Scene.OrbitCamera->SetPosition(SVector3f(sin(Speed*OrbitCameraTimer)*Distance, 0.4f, cos(Speed*OrbitCameraTimer)*Distance));
	Scene.OrbitCamera->SetLookAtTarget(vec3f(0, -0.5f, 0));
	OrbitCameraTimer += Increment;

	//Scene.LightPosition = SceneManager->GetScene()->GetActiveCamera()->GetPosition() + SVector3f(0, 0, 0);

	//SceneManager->GetDefaultColorRenderPass()->onPreDraw();
	//glClearColor(1.f, 0.25f, 0.05f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.15f, 0.45f, 0.5f, 1.0f);

	glDisable(GL_DEPTH_TEST);
	Context->Scene.SkyBox->SetPosition(SceneManager->GetScene()->GetActiveCamera()->GetPosition());
	Context->Scene.SkyBox->Update();
	Context->Scene.SkyBox->UpdateAbsoluteTransformation();

	//Context->Scene.SkyBox->load(SceneManager, SceneManager->getDefaultColorRenderPass());
	//Context->Scene.SkyBox->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);
	glEnable(GL_DEPTH_TEST);

	Scene.Volume->Update();

	SceneManager->DrawAll();

	if (Context->Window->IsKeyDown(EKey::F1))
		CFrameBuffer::DrawTextureToScreen(Context->SceneColorTexture);
	else if (Context->Window->IsKeyDown(EKey::F2))
		CFrameBuffer::DrawTextureToScreen(Context->SceneRefractColor);
	else if (Context->Window->IsKeyDown(EKey::F3))
		CFrameBuffer::DrawTextureToScreen(Context->SceneDepthBuffer);
	else if (Context->Window->IsKeyDown(EKey::F4))
		CFrameBuffer::DrawTextureToScreen(Context->RefractMaskTexture);
	else if (Context->Window->IsKeyDown(EKey::F5))
		CFrameBuffer::DrawTextureToScreen(Context->FinalColor);
	else
		CFrameBuffer::DrawTextureToScreen(Context->AAColor);

	if (! ShowDepth)
	{
		//Context->Scene.Volume->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);

		//glEnable(GL_BLEND);
		//glDepthMask(false);
		//Context->Scene.Water->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);
		//glDepthMask(true);

		//SceneManager->endDraw();
	}

	//if (ShowDepth)
	//{
	//	CFrameBufferObject::bindDeviceBackBuffer();
	//	glDisable(GL_DEPTH_TEST);
	//	{
	//		CShaderContext Context(* CShaderLoader::loadShader("FBO/QuadCopy"));

	//		Context.bindTexture("uTexColor", SceneManager->getSceneDepthTexture());
	//		Context.bindBufferObject("aPosition", CSceneManager::getQuadHandle(), 2);

	//		glDrawArrays(GL_QUADS, 0, 4);
	//	}
	//	glEnable(GL_DEPTH_TEST);
	//}

	if (GUIEnabled)
		Context->GUIContext->Manager->Draw(Elapsed, false);
	
	if (ShowKey && Font)
	{
		auto GetValueAt = [](f32 const v)
		{
			color4f Color = CSpectrumColorMapper::MapColor(v);
			glColor3f(Color.Red, Color.Green, Color.Blue);
			return GlobalMin * (1 - v) + GlobalMax * v;
		};
		int Counter = 10;
		auto DrawColor = [this, & Counter, GetValueAt](c8 const * const Label, f32 const v)
		{
			f32 Value = GetValueAt(v);
			Font->print(10, Counter, "%s:", Label);
			Font->print(120, Counter, "%.3f", Value);
			Counter += 25;
		};
		DrawColor("Black", 0.f);
		DrawColor("Blue", 0.15f);
		DrawColor("Green", 0.4f);
		DrawColor("Yellow", 0.6f);
		DrawColor("Orange", 0.7f);
		DrawColor("Red", 0.8f);
		DrawColor("White", 1.f);
	}
	
	if (Context->Window->IsKeyDown(EKey::Grave) && Font)
	{
		Font->print(10, 10, "Camera Position: %.3f %.3f %.3f\n", Scene.Camera->GetPosition().X, Scene.Camera->GetPosition().Y, Scene.Camera->GetPosition().Z);
	}
	
    // Read screen colors
	if (gifWriter)
	{
		u32 const FrameWidth = Context->Window->GetSize().X;
		u32 const FrameHeight = Context->Window->GetSize().Y;
		unsigned char * ImageData = new unsigned char[FrameWidth * FrameHeight * 3];

		static u32 Counter = 0;
		glReadPixels(0, 0, FrameWidth, FrameHeight, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
		gifWriter->AddFrame(ImageData, 0.12f);

		delete [] ImageData;

		if (Speed*OrbitCameraTimer >= 2*Constants32::Pi)
			EndGifDraw();
	}
}

void CMainState::AddConsoleMessage(std::string const & Message, color4i const & Color)
{
	Context->GUIContext->GetConsole()->AddMessage(Message, Color);
}

void CMainState::CalculateDataAlignment()
{
	CProgramContext::SScene & Scene = Context->Scene;

	CSite * CurrentSite = Context->CurrentSite;
	CDataSet const * const DataSet = CurrentSite->GetCurrentDataSet();
	STable & Points = CurrentSite->GetCurrentDataSet()->Points;
	
	longlatd MapLonLatMin;
	longlatd MapLonLatMax;

	if (CurrentSite->GetCurrentLocation())
	{
		MapLonLatMin = (CurrentSite->GetCurrentLocation()->LowerBound);
		MapLonLatMax = (CurrentSite->GetCurrentLocation()->UpperBound);
	}

	SRange<f64> XRange = Points.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionXField, 15.0);
	SRange<f64> YRange = Points.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionYField, 15.0);
	SRange<f64> ZRange = Points.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionZField, 15.0);
	
	if (XRange.IsEmpty())
		XRange = SRange<f64>(MapLonLatMin.Longitude, MapLonLatMax.Longitude);
	if (YRange.IsEmpty())
		YRange = SRange<f64>(-1, 1);
	if (ZRange.IsEmpty())
		ZRange = SRange<f64>(MapLonLatMin.Latitude, MapLonLatMax.Latitude);

	printf("Longlat range is %g %g to %g %g\n", XRange.Minimum, ZRange.Minimum, XRange.Maximum, ZRange.Maximum);

	longlatd const DataLonLatMin(XRange.Minimum, ZRange.Minimum), DataLonLatMax(XRange.Maximum, ZRange.Maximum);
	longlatd const DataLonLatCenter = (DataSet->ManuallySetDataLongLat ? DataSet->DataLonLatCenter : (DataLonLatMin + DataLonLatMax) / 2.f);
	
	vec2d DataRangeMin, DataRangeMax, MapRangeMin, MapRangeMax;
	sharedPtr<longlatd::IProjectionSystem> Projection;
	if (ProjectionMode == 0)
		Projection = sharedNew(new longlatd::CHaversineProjection());
	else if (ProjectionMode == 1)
		Projection = sharedNew(new longlatd::CVincentyProjection());
	else if (ProjectionMode == 2)
		Projection = sharedNew(new longlatd::CEquirectangularProjection(DataLonLatCenter.Latitude));
	DataRangeMin = DataLonLatCenter.OffsetTo(DataLonLatMin, Projection);
	DataRangeMax = DataLonLatCenter.OffsetTo(DataLonLatMax, Projection);
	if (CurrentSite->GetCurrentLocation())
	{
		MapRangeMin = DataLonLatCenter.OffsetTo(MapLonLatMin, Projection);
		MapRangeMax = DataLonLatCenter.OffsetTo(MapLonLatMax, Projection);
	}
	else
	{
		MapRangeMin = DataLonLatCenter.OffsetTo(DataLonLatMin, Projection);
		MapRangeMax = DataLonLatCenter.OffsetTo(DataLonLatMax, Projection);
	}

	vec2d const DataRangeSize = (DataSet->ManuallySetDataLongLat ? DataLonLatMax - DataLonLatMin : DataRangeMax - DataRangeMin);
	vec2d const DataRangeCenter = (DataSet->ManuallySetDataLongLat ? DataRangeSize / 2 : (DataRangeMin + DataRangeMax) / 2.f);
	f64 const DataDepth = YRange.Size();
	
	vec2d const MapRangeSize = MapRangeMax - MapRangeMin;
	vec2d const MapRangeCenter = (MapRangeMin + MapRangeMax) / 2.f;
	f64 const MapDepth = DataSet->MapDepth;
	
	printf("Data range is %f by %f meters,\n", DataRangeSize.X, DataRangeSize.Y);
	printf("Terrain range is %f by %f meters,\n", MapRangeSize.X, MapRangeSize.Y);
	
	vec2d const ActualOffset = MapRangeCenter - DataRangeCenter;
	vec2d const MapOffset = ActualOffset * 3.f / Maximum(DataRangeSize.X, DataRangeSize.Y);
	vec3d const DataScale = 3.0 * vec3d(DataRangeSize.X, DataDepth, DataRangeSize.Y) / Maximum(DataRangeSize.X, DataRangeSize.Y);
	vec3d const MapScale = DataScale * vec3d(MapRangeSize.X, MapDepth, MapRangeSize.Y) / vec3d(DataRangeSize.X, DataDepth, DataRangeSize.Y);

	static f64 const YExaggeration = DataSet->YExaggeration;
	static vec3d const Multiplier = vec3d(1, YExaggeration, 1);
	
	Scene.Glyphs->GetNode()->SetScale(DataScale * Multiplier);
	Scene.Volume->GetNode()->SetScale(DataScale * Multiplier);
	Scene.Glyphs->GetNode()->SetTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, 0));
	Scene.Volume->GetNode()->SetTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, 0));
	
	Scene.Terrain->GetNode()->SetScale(MapScale * Multiplier / CTerrainNodeManager::Size);
	//Scene.Water->SetScale(MapScale / CTerrainNodeManager::Size);

	Scene.Terrain->GetNode()->SetTranslation(vec3f(MapOffset.X, 0, -MapOffset.Y));
	//Scene.Water->SetTranslation(vec3f(MapOffset.X, 0, -MapOffset.Y));

	//// Flip for RHC->LHC
	Scene.Glyphs->GetNode()->SetScale(Scene.Glyphs->GetNode()->GetScale() * vec3f(1, 1, -1));
	Scene.Volume->GetNode()->SetScale(Scene.Volume->GetNode()->GetScale() * vec3f(1, 1, -1));
	Scene.Terrain->GetNode()->SetScale(Scene.Terrain->GetNode()->GetScale() * vec3f(1, 1, -1));
	//Scene.Water->SetScale(Scene.Water->GetScale() * vec3f(1, 1, -1));

	//// Flip Height -> Depth
	Scene.Volume->GetNode()->SetScale(Scene.Volume->GetNode()->GetScale() * vec3f(1, -1, 1));
	Scene.Glyphs->GetNode()->SetScale(Scene.Glyphs->GetNode()->GetScale() * vec3f(1, -1, 1));
}

void CMainState::SetSite(int site)
{
	CProgramContext::SScene & Scene = Context->Scene;
	Context->CurrentSite->SetSelectedLocation(site);
	Context->CurrentSite->InitSceneElements(Context->Scene);
	CalculateDataAlignment();
}
