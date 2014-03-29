
#include "CMainState.h"

#include "CTerrainSceneObject.h"
#include "CGlyphSceneObject.h"
#include "CWaterSceneObject.h"
#include "CSite.h"
#include "CSpectrumColorMapper.h"

#include <ionScience.h>


CMainState::CMainState()
	: Scale(1), Mode(0), ShowDepth(false), ProjectionMode(0), GUIEnabled(true), OrbitCameraTimer(0), gifWriter()
{}

void CMainState::Begin()
{
	Context->GUIContext->SetupMainState();

	Context->Scene.Timer = 0.f;
	Context->Scene.Glyphs->BuildLines();

	ReflectionRenderPass = sharedNew<CReflectionRenderPass>(new CReflectionRenderPass);
	SceneManager->getEffectManager()->RenderPasses.push_back(ReflectionRenderPass);

	CalculateDataAlignment();
	OrbitCameraTimer = 0;

	Font.init("OpenSans.ttf", 18);
}

void CMainState::End()
{
	Context->GUIContext->Clear();
}

void CMainState::BeginGifDraw()
{
	SceneManager->setActiveCamera(Context->Scene.OrbitCamera);
	gifWriter = new GifWriter(SceneManager->getScreenSize());
	OrbitCameraTimer = 0;
}

void CMainState::EndGifDraw()
{
	SceneManager->setActiveCamera(Context->Scene.Camera);
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

	Scene.Camera->Update(Elapsed);

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
	Scene.Camera->UpdateProjection();

	Scene.Timer += Elapsed * 0.16f;

	float const Distance = 2.5f;
	static f32 const Speed = 1.f;
	static f32 const Increment = 0.05f;
	Scene.OrbitCamera->setPosition(SVector3f(sin(Speed*OrbitCameraTimer)*Distance, 0.4f, cos(Speed*OrbitCameraTimer)*Distance));
	Scene.OrbitCamera->SetLookAtTarget(vec3f(0, -0.5f, 0));
	OrbitCameraTimer += Increment;

	Scene.LightPosition = SceneManager->getActiveCamera()->getPosition() + SVector3f(0, 0, 0);

	SceneManager->drawAll();

	if (! ShowDepth)
	{
		Context->Scene.Volume->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);

		glEnable(GL_BLEND);
		glDepthMask(false);
		Context->Scene.Water->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);
		glDepthMask(true);

		SceneManager->endDraw();
	}

	if (ShowDepth)
	{
		CFrameBufferObject::bindDeviceBackBuffer();
		glDisable(GL_DEPTH_TEST);
		{
			CShaderContext Context(* CShaderLoader::loadShader("FBO/QuadCopy"));

			Context.bindTexture("uTexColor", SceneManager->getSceneDepthTexture());
			Context.bindBufferObject("aPosition", CSceneManager::getQuadHandle(), 2);

			glDrawArrays(GL_QUADS, 0, 4);
		}
		glEnable(GL_DEPTH_TEST);
	}

	if (GUIEnabled)
		Context->GUIContext->Draw(Elapsed, false);
	
	//auto GetValueAt = [](f32 const v)
	//{
	//	color4f Color = CSpectrumColorMapper::MapColor(v);
	//	glColor3f(Color.Red, Color.Green, Color.Blue);
	//	return GlobalMin * (1 - v) + GlobalMax * v;
	//};
	//int Counter = 10;
	//auto DrawColor = [this, & Counter, GetValueAt](c8 const * const Label, f32 const v)
	//{
	//	f32 Value = GetValueAt(v);
	//	freetype::print(Font, 10, Counter, "%s:", Label);
	//	freetype::print(Font, 120, Counter, "%.3f °C", Value);
	//	Counter += 25;
	//};
	//DrawColor("Black", 0.f);
	//DrawColor("Blue", 0.15f);
	//DrawColor("Green", 0.4f);
	//DrawColor("Yellow", 0.6f);
	//DrawColor("Orange", 0.7f);
	//DrawColor("Red", 0.8f);
	//DrawColor("White", 1.f);
	
    // Read screen colors
	if (gifWriter)
	{
		u32 const FrameWidth = Application->GetWindow().GetSize().X;
		u32 const FrameHeight = Application->GetWindow().GetSize().Y;
		unsigned char * ImageData = new unsigned char[FrameWidth * FrameHeight * 3];

		static u32 Counter = 0;
		glReadPixels(0, 0, FrameWidth, FrameHeight, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
		gifWriter->AddFrame(ImageData, 0.12f);

		delete [] ImageData;

		if (Speed*OrbitCameraTimer >= 2*Constants32::Pi)
			EndGifDraw();
	}

	CApplication::Get().GetWindow().SwapBuffers();
}

void CMainState::AddConsoleMessage(std::string const & Message, color4i const & Color)
{
	Context->GUIContext->GetConsole()->AddMessage(Message, Color);
}

void CMainState::CalculateDataAlignment()
{
	CProgramContext::SScene & Scene = Context->Scene;

	CSite * CurrentSite = Context->CurrentSite;
	STable & DataSet = CurrentSite->GetCurrentDataSet()->Points;

	SRange<f64> XRange = DataSet.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionXField, 15.0);
	SRange<f64> YRange = DataSet.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionYField, 15.0);
	SRange<f64> ZRange = DataSet.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionZField, 15.0);
	
	if (XRange.IsEmpty())
		XRange = SRange<f64>(-1, 1);
	if (YRange.IsEmpty())
		YRange = SRange<f64>(-1, 1);
	if (ZRange.IsEmpty())
		ZRange = SRange<f64>(-1, 1);

	printf("Longlat range is %g %g to %g %g\n", XRange.Minimum, ZRange.Minimum, XRange.Maximum, ZRange.Maximum);

	longlatd const DataLonLatMin(XRange.Minimum, ZRange.Minimum), DataLonLatMax(XRange.Maximum, ZRange.Maximum);
	longlatd const MapLonLatMin(CurrentSite->GetCurrentLocation()->LowerBound), MapLonLatMax(CurrentSite->GetCurrentLocation()->UpperBound);

	longlatd const DataLonLatCenter = longlatd(SLongitudeLatitude<f64>::DMStoDecimal(9, 32, 23.44 - 3.1), SLongitudeLatitude<f64>::DMStoDecimal(63, 35, 35.37 - 2.4));// (DataLonLatMin + DataLonLatMax) / 2.f;
	
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
	MapRangeMin = DataLonLatCenter.OffsetTo(MapLonLatMin, Projection);
	MapRangeMax = DataLonLatCenter.OffsetTo(MapLonLatMax, Projection);

	vec2d const DataRangeSize = DataLonLatMax - DataLonLatMin;// DataRangeMax - DataRangeMin;
	vec2d const DataRangeCenter = DataRangeSize / 2;// (DataLonLatMin + DataLonLatMax) / 2.f;// (DataRangeMin + DataRangeMax) / 2.f;
	f64 const DataDepth = YRange.Size();
	
	vec2d const MapRangeSize = MapRangeMax - MapRangeMin;
	vec2d const MapRangeCenter = (MapRangeMin + MapRangeMax) / 2.f;
	f64 const MapDepth = 180.0;
	
	printf("Data range is %f by %f meters,\n", DataRangeSize.X, DataRangeSize.Y);
	printf("Terrain range is %f by %f meters,\n", MapRangeSize.X, MapRangeSize.Y);
	
	vec2d const ActualOffset = MapRangeCenter - DataRangeCenter;
	vec2d const MapOffset = ActualOffset * 3.f / Maximum(DataRangeSize.X, DataRangeSize.Y);
	vec3d const DataScale = 3.0 * vec3d(DataRangeSize.X, DataDepth, DataRangeSize.Y) / Maximum(DataRangeSize.X, DataRangeSize.Y);
	vec3d const MapScale = DataScale * vec3d(MapRangeSize.X, MapDepth, MapRangeSize.Y) / vec3d(DataRangeSize.X, DataDepth, DataRangeSize.Y);

	static f64 const YExaggeration = 8.0;
	static vec3d const Multiplier = vec3d(1, YExaggeration, 1);
	
	Scene.Glyphs->setScale(DataScale * Multiplier);
	Scene.Volume->setScale(DataScale * Multiplier);
	Scene.Glyphs->setTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, 0));
	Scene.Volume->setTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, 0));
	
	Scene.Terrain->setScale(MapScale * Multiplier / CTerrainSceneObject::Size);
	Scene.Water->setScale(MapScale / CTerrainSceneObject::Size);
	Scene.SkyBox->setScale(SVector3f(MapScale.X, 30.f, MapScale.Z));

	Scene.Terrain->setTranslation(vec3f(MapOffset.X, 0, -MapOffset.Y));
	Scene.Water->setTranslation(vec3f(MapOffset.X, 0, -MapOffset.Y));
	Scene.SkyBox->setTranslation(vec3f(MapOffset.X, 0, -MapOffset.Y));
	
	// Flip for RHC->LHC
	Scene.Glyphs->setScale(Scene.Glyphs->getScale() * vec3f(1, 1, -1));
	Scene.Volume->setScale(Scene.Volume->getScale() * vec3f(1, 1, -1));
	Scene.Terrain->setScale(Scene.Terrain->getScale() * vec3f(1, 1, -1));
	Scene.Water->setScale(Scene.Water->getScale() * vec3f(1, 1, -1));
	Scene.SkyBox->setScale(Scene.SkyBox->getScale() * vec3f(1, 1, -1));
	
	// Flip Height -> Depth
	Scene.Volume->setScale(Scene.Volume->getScale() * vec3f(1, -1, 1));
	Scene.Glyphs->setScale(Scene.Glyphs->getScale() * vec3f(1, -1, 1));
}

void CMainState::SetSite(int site)
{
	CProgramContext::SScene & Scene = Context->Scene;
	//Scene.Terrain->SetSite(Site);
	CalculateDataAlignment();
}
