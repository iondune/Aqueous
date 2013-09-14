
#include "CMainState.h"

#include "CTerrainSceneObject.h"
#include "CGlyphSceneObject.h"
#include "CWaterSceneObject.h"
#include "SciDataManager.h"

#include <ionScience.h>


CMainState::CMainState()
	: Scale(1), Mode(0), ShowDepth(false), Site(0), ProjectionMode(0)
{}

void CMainState::Begin()
{
	Context->GUIContext->SetupMainState();

	Context->Scene.Timer = 0.f;
	Context->Scene.Glyphs->BuildLines();

	ReflectionRenderPass = sharedNew<CReflectionRenderPass>(new CReflectionRenderPass);
	SceneManager->getEffectManager()->RenderPasses.push_back(ReflectionRenderPass);

	CalculateDataAlignment();
}

void CMainState::End()
{
	Context->GUIContext->Clear();
}

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

	float const Distance = 4.f;
	Scene.OrbitCamera->setPosition(SVector3f(sin(Scene.Timer)*Distance, 2.3f, cos(Scene.Timer)*Distance));
	Scene.OrbitCamera->SetLookAtTarget(SVector3f());

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

	Context->GUIContext->Draw(Elapsed, false);

	CApplication::Get().GetWindow().SwapBuffers();
}

void CMainState::AddConsoleMessage(std::string const & Message, color4i const & Color)
{
	Context->GUIContext->GetConsole()->AddMessage(Message, Color);
}

void CMainState::CalculateDataAlignment()
{
	CProgramContext::SScene & Scene = Context->Scene;

	SciDataManager * DataManager = Context->DataManager;
	STable & DataSet = DataManager->GetRawValues();

	Range XRange = DataSet.GetFieldRange(DataManager->GetRawValues().Traits.PositionXField, 15.0);
	Range YRange = DataSet.GetFieldRange(DataManager->GetRawValues().Traits.PositionYField, 15.0);
	Range ZRange = DataSet.GetFieldRange(DataManager->GetRawValues().Traits.PositionZField, 15.0);

	
	printf("Longlat range is %f %f to %f %f\n", XRange.first, ZRange.first, XRange.second, ZRange.second);

	longlatf const DataLonLatMin(XRange.first, ZRange.first), DataLonLatMax(XRange.second, ZRange.second);
	longlatf MapLonLatMin, MapLonLatMax;
	switch (Site)
	{
	default:
	case 0:
		MapLonLatMin.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 55, 45.32f);
		MapLonLatMin.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 38, 17.22f);
		MapLonLatMax.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 2, 34.80f);
		MapLonLatMax.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 41, 59.01f);
		break;
	case 1:
		MapLonLatMin.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 49, 27.68f);
		MapLonLatMin.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 34, 20.96f);
		MapLonLatMax.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 11, 1.75f);
		MapLonLatMax.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 46, 11.45f);
		break;
	case 2:
		MapLonLatMin.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 54, 13.29f);
		MapLonLatMin.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 30, 37.33f);
		MapLonLatMax.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 27, 10.16f);
		MapLonLatMax.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 48, 48.57f);
		break;
	case 3:
		MapLonLatMin.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 39, 1.38f);
		MapLonLatMin.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 37, 13.75f);
		MapLonLatMax.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 17, 17.79f);
		MapLonLatMax.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 58, 5.72f);
		break;
	case 4:
		MapLonLatMin.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(9, 4, 33.9f);
		MapLonLatMin.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(56, 13, 56.68f);
		MapLonLatMax.Longitude = SLongitudeLatitude<f32>::DMStoDecimal(10, 53, 14.45f);
		MapLonLatMax.Latitude = SLongitudeLatitude<f32>::DMStoDecimal(57, 13, 29.39f);
		break;
	}

	longlatf const DataLonLatCenter = (DataLonLatMin + DataLonLatMax) / 2.f;
	
	vec2f DataRangeMin, DataRangeMax, MapRangeMin, MapRangeMax;
	sharedPtr<longlatf::IProjectionSystem> Projection;
	if (ProjectionMode == 0)
	{
		Projection = sharedNew(new longlatf::CHaversineProjection());
	}
	else if (ProjectionMode == 1)
	{
		Projection = sharedNew(new longlatf::CVincentyProjection());
	}
	else if (ProjectionMode == 2)
	{
		Projection = sharedNew(new longlatf::CEquirectangularProjection(DataLonLatCenter.Latitude));
	}
	DataRangeMin = DataLonLatCenter.OffsetTo(DataLonLatMin, Projection);
	DataRangeMax = DataLonLatCenter.OffsetTo(DataLonLatMax, Projection);
	MapRangeMin = DataLonLatCenter.OffsetTo(MapLonLatMin, Projection);
	MapRangeMax = DataLonLatCenter.OffsetTo(MapLonLatMax, Projection);

	vec2f const DataRangeSize = DataRangeMax - DataRangeMin;
	vec2f const DataRangeCenter = (DataRangeMin + DataRangeMax) / 2.f;
	f32 const DataDepth = (YRange.second - YRange.first);
	
	vec2f const MapRangeSize = MapRangeMax - MapRangeMin;
	vec2f const MapRangeCenter = (MapRangeMin + MapRangeMax) / 2.f;
	f32 const MapDepth = 800.f;
	
	printf("Data range is %f by %f meters,\n", DataRangeSize.X, DataRangeSize.Y);
	printf("Terrain range is %f by %f meters,\n", MapRangeSize.X, MapRangeSize.Y);
	
	vec2f const ActualOffset = MapRangeCenter - DataRangeCenter;
	vec2f const MapOffset = ActualOffset * 3.f / Maximum(DataRangeSize.X, DataRangeSize.Y);
	vec3f const DataScale = 3.f * vec3f(DataRangeSize.X, DataDepth, DataRangeSize.Y) / Maximum(DataRangeSize.X, DataRangeSize.Y);
	vec3f const MapScale = DataScale * vec3f(MapRangeSize.X, MapDepth, MapRangeSize.Y) / vec3f(DataRangeSize.X, DataDepth, DataRangeSize.Y);

	static f32 const YExaggeration = 3.f;
	static vec3f const Multiplier = vec3f(1, YExaggeration, 1);
	
	Scene.Glyphs->setScale(DataScale * Multiplier);
	Scene.Volume->setScale(DataScale * Multiplier);
	Scene.Glyphs->setTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, -1));
	Scene.Volume->setTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, -1));

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
}

void CMainState::SetSite(int site)
{
	Site = site;

	CProgramContext::SScene & Scene = Context->Scene;
	Scene.Terrain->SetSite(Site);
	CalculateDataAlignment();
}
