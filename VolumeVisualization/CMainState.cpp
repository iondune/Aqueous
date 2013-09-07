#include "CMainState.h"

#include "CTerrainSceneObject.h"
#include "CGlyphSceneObject.h"
#include "CWaterSceneObject.h"


CMainState::CMainState()
	: Scale(1), Mode(0), ShowDepth(false), Site(0), MeterMode(0)
{}

void CMainState::Begin()
{
	Context->GUIContext->SetupMainState();

	Context->Scene.Timer = 0.f;
	Context->Scene.Glyphs->buildLines();

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
	//SceneManager->endDraw();

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
	else
		Context->Scene.Volume->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);

	SceneManager->getSceneFrameBuffer()->bind();
	glEnable(GL_BLEND);
	glDepthMask(false);
	Context->Scene.Water->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);
	glDepthMask(true);
	SceneManager->endDraw();

	Context->GUIContext->Draw(Elapsed, false);

	CApplication::Get().GetWindow().SwapBuffers();
}

void CMainState::addConsoleMessage(std::string const & Message, Gwen::Color const & Color)
{
	Context->GUIContext->GetConsole()->addMessage(Message, Color);
}

static double const toRadians(double const deg)
{
	return deg * Constants64::Pi / 180;
}

static double DistFrom(double lat1, double lng1, double lat2, double lng2)
{
	double earthRadius = 6371.0;
	double dLat = toRadians(lat2-lat1);
	double dLng = toRadians(lng2-lng1);
	double a = 
		sin(dLat/2) * sin(dLat/2) +
		cos(toRadians(lat1)) * cos(toRadians(lat2)) * sin(dLng/2) * sin(dLng/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double dist = earthRadius * c;

	int meterConversion = 1000;

	return (dist * meterConversion);
}

static double DistFromLat(double lat1, double lat2, double lng)
{
	return DistFrom(lat1, lng, lat2, lng);
}

static double DistFromLong(double lng1, double lng2, double lat)
{
	return DistFrom(lat, lng1, lat, lng2);
}

static vec2f GetLongLatAreaDimensions(vec2f const & Min, vec2f const & Max)
{
	vec2f const Center = (Min + Max) / 2.f;
	return vec2f(
		DistFromLong(Min.X, Max.X, Center.Y),
		DistFromLat(Min.Y, Max.Y, Center.X));
}

enum class ECompassDirection
{
	N = 1, E = 1,
	S = -1, W = -1
};

static f64 LongLatDecimalDegrees(f64 const Deg, f64 const Min, f64 const Sec, ECompassDirection const Direction = ECompassDirection::N)
{
	return (Deg + Min/60.0 + Sec/3600.0) * (int) Direction;
}

static f64 LongLatDecimalDegrees(std::string const & String)
{
	f64 Deg, Min, Sec;
	char Dir, Dummy;
	ECompassDirection Direction;

	sscanf(String.c_str(), "%lf %c %lf %c %lf %c %c", & Deg, & Dummy, & Min, & Dummy, & Sec, & Dummy, & Dir);
	
	switch (tolower(Dir))
	{
	default:
	case 'N':
		Direction = ECompassDirection::N;
		break;
	case 'E':
		Direction = ECompassDirection::E;
		break;
	case 'S':
		Direction = ECompassDirection::S;
		break;
	case 'W':
		Direction = ECompassDirection::W;
		break;
	}

	return LongLatDecimalDegrees(Deg, Min, Sec, Direction);
}

#include "SciDataManager.h"

void CMainState::CalculateDataAlignment()
{
	CProgramContext::SScene & Scene = Context->Scene;

	SciDataManager * DataManager = Context->DataManager;
	STable & DataSet = DataManager->GetRawValues();

	Range XRange = DataSet.GetFieldRange(DataManager->GetRawValues().Traits.PositionXField, 15.0);
	Range YRange = DataSet.GetFieldRange(DataManager->GetRawValues().Traits.PositionYField, 15.0);
	Range ZRange = DataSet.GetFieldRange(DataManager->GetRawValues().Traits.PositionZField, 15.0);

	vec2f const DataLonLatMin(XRange.first, ZRange.first), DataLonLatMax(XRange.second, ZRange.second);
	vec2f MapLonLatMin, MapLonLatMax;
	switch (Site)
	{
	default:
	case 0:
		MapLonLatMin = vec2f(LongLatDecimalDegrees(9, 55, 45.32), LongLatDecimalDegrees(56, 38, 17.22));
		MapLonLatMax = vec2f(LongLatDecimalDegrees(10, 2, 34.80), LongLatDecimalDegrees(56, 41, 59.01));
		break;
	case 1:
		MapLonLatMin = vec2f(LongLatDecimalDegrees(9, 49, 27.68), LongLatDecimalDegrees(56, 34, 20.96));
		MapLonLatMax = vec2f(LongLatDecimalDegrees(10, 11, 1.75), LongLatDecimalDegrees(56, 46, 11.45));
		break;
	case 2:
		MapLonLatMin = vec2f(LongLatDecimalDegrees(9, 54, 13.29), LongLatDecimalDegrees(56, 30, 37.33));
		MapLonLatMax = vec2f(LongLatDecimalDegrees(10, 27, 10.16), LongLatDecimalDegrees(56, 48, 48.57));
		break;
	case 3:
		MapLonLatMin = vec2f(LongLatDecimalDegrees(9, 39, 1.38), LongLatDecimalDegrees(56, 37, 13.75));
		MapLonLatMax = vec2f(LongLatDecimalDegrees(10, 17, 17.79), LongLatDecimalDegrees(56, 58, 5.72));
		break;
	}

	vec2f const DataLonLatCenter = (DataLonLatMin + DataLonLatMax) / 2.f;
	
	vec2f DataRangeMin;
	vec2f DataRangeMax;
	vec2f MapRangeMin;
	vec2f MapRangeMax;
	if (MeterMode == 0)
	{
	DataRangeMin = -vec2f(DistFromLong(DataLonLatMin.X, DataLonLatCenter.X, DataLonLatCenter.Y), DistFromLat(DataLonLatMin.Y, DataLonLatCenter.Y, DataLonLatCenter.X));
	DataRangeMax = vec2f(DistFromLong(DataLonLatCenter.X, DataLonLatMax.X, DataLonLatCenter.Y), DistFromLat(DataLonLatCenter.Y, DataLonLatMax.Y, DataLonLatCenter.X));
	MapRangeMin = -vec2f(DistFromLong(MapLonLatMin.X, DataLonLatCenter.X, DataLonLatCenter.Y), DistFromLat(MapLonLatMin.Y, DataLonLatCenter.Y, DataLonLatCenter.X));
	MapRangeMax = vec2f(DistFromLong(DataLonLatCenter.X, MapLonLatMax.X, DataLonLatCenter.Y), DistFromLat(DataLonLatCenter.Y, MapLonLatMax.Y, DataLonLatCenter.X));
	}
	else if (MeterMode == 1)
	{
		DataRangeMin = -vec2f(DistFromLong(DataLonLatMin.X, DataLonLatCenter.X, Average(DataLonLatMin.Y, DataLonLatCenter.Y)), DistFromLat(DataLonLatMin.Y, DataLonLatCenter.Y, Average(DataLonLatMin.X, DataLonLatCenter.X)));
		DataRangeMax = vec2f(DistFromLong(DataLonLatCenter.X, DataLonLatMax.X, Average(DataLonLatMax.Y, DataLonLatCenter.Y)), DistFromLat(DataLonLatCenter.Y, DataLonLatMax.Y, Average(DataLonLatMax.X, DataLonLatCenter.X)));
		MapRangeMin = -vec2f(DistFromLong(MapLonLatMin.X, DataLonLatCenter.X, Average(MapLonLatMin.Y, DataLonLatCenter.Y)), DistFromLat(MapLonLatMin.Y, DataLonLatCenter.Y, Average(MapLonLatMin.X, DataLonLatCenter.X)));
		MapRangeMax = vec2f(DistFromLong(DataLonLatCenter.X, MapLonLatMax.X, Average(MapLonLatMax.Y, DataLonLatCenter.Y)), DistFromLat(DataLonLatCenter.Y, MapLonLatMax.Y, Average(MapLonLatMax.X, DataLonLatCenter.X)));
	}
	else if (MeterMode == 2)
	{
		DataRangeMin = -vec2f(DistFromLong(DataLonLatMin.X, DataLonLatCenter.X, DataLonLatMin.Y), DistFromLat(DataLonLatMin.Y, DataLonLatCenter.Y, DataLonLatMin.X));
		DataRangeMax = vec2f(DistFromLong(DataLonLatCenter.X, DataLonLatMax.X, DataLonLatMax.Y), DistFromLat(DataLonLatCenter.Y, DataLonLatMax.Y, DataLonLatMax.X));
		MapRangeMin = -vec2f(DistFromLong(MapLonLatMin.X, DataLonLatCenter.X, MapLonLatMin.Y), DistFromLat(MapLonLatMin.Y, DataLonLatCenter.Y, MapLonLatMin.X));
		MapRangeMax = vec2f(DistFromLong(DataLonLatCenter.X, MapLonLatMax.X, MapLonLatMax.Y), DistFromLat(DataLonLatCenter.Y, MapLonLatMax.Y, MapLonLatMax.X));
	}

	vec2f const DataRangeSize = DataRangeMax - DataRangeMin;
	vec2f const DataRangeCenter = (DataRangeMin + DataRangeMax) / 2.f;
	//vec2f const DataActualSize(GetLongLatAreaDimensions(DataRangeMin, DataRangeMax));
	f32 const DataDepth = YRange.second - YRange.first;

	//vec2f const MapRangeMin(DataRangeCenter - 2*(DataRangeCenter - DataRangeMin)), MapRangeMax(DataRangeCenter + 2*(DataRangeMax - DataRangeCenter));
	
	vec2f const MapRangeSize = MapRangeMax - MapRangeMin;
	vec2f const MapRangeCenter = (MapRangeMin + MapRangeMax) / 2.f;
	//vec2f const MapActualSize(GetLongLatAreaDimensions(MapRangeMin, MapRangeMax));
	f32 const MapDepth = 800.f;

	printf("Data range is %f by %f meters,\n", DataRangeSize.X, DataRangeSize.Y);
	
	vec2f const ActualOffset = MapRangeCenter - DataRangeCenter;//vec2f(DistFromLong(DataRangeCenter.X, MapRangeCenter.X, DataRangeCenter.Y), DistFromLat(DataRangeCenter.Y, MapRangeCenter.Y, DataRangeCenter.X));
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

	/*vec2f ScaleAdjust = (MapRangeMax - MapRangeMin) / (DataRangeMax - DataRangeMin);
	f32 MaxAdjust = Maximum(ScaleAdjust.X, ScaleAdjust.Y);
	vec2f TranslationAdjust = (DataRangeCenter - MapRangeMin) / (MapRangeMax - DataRangeCenter);

	vec2f RelativeTranslate;
	for (int i = 0; i < 2; ++ i)
	{
		if (TranslationAdjust[i] > 1.f)
		{
			TranslationAdjust[i] = 1.f / TranslationAdjust[i];
			RelativeTranslate[i] = -(1.f - TranslationAdjust[i]);
		}
		else
		{
			RelativeTranslate[i] = (1.f - TranslationAdjust[i]);
		}
	}

	RelativeTranslate *= MaxAdjust;

	Scene.Terrain->setTranslation(vec3f(RelativeTranslate.X, 0.f, -RelativeTranslate.Y));
	Scene.Water->setTranslation(vec3f(RelativeTranslate.X, 0.f, -RelativeTranslate.Y));*/
}

void CMainState::SetSite(int site)
{
	Site = site;

	CProgramContext::SScene & Scene = Context->Scene;
	Scene.Terrain->SetSite(Site);
	CalculateDataAlignment();
}
