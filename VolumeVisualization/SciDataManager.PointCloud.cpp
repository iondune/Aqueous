#include "SciDataManager.h"

#include "CProgramContext.h"
#include <CInstanceSceneObject.h>


void SciDataManager::createPointCloudObjects(bool FromRaw, ISceneObject * RootParent, SVector3f const DataScale, IColorMapper * ColorMapper, 
		std::string const & xField, std::string const & yField, std::string const & zField)
{
	SciDataSet & DataSet = FromRaw ? RawValues : GridValues;

	ColorMapper->preProcessValues(DataSet);

	Range	XRange = DataSet.getValueRange(xField, 5.0);
	Range	YRange = DataSet.getValueRange(yField, 5.0);
	Range	ZRange = DataSet.getValueRange(zField, 5.0);

	CMeshInstanceSceneObject * Container = new CMeshInstanceSceneObject();
	Container->setParent(RootParent);
	Container->setMesh(CProgramContext::get().Scene.Cube);
	Container->setScale(SVector3f(1.f) / 32.f * (vec3f(1.f) / RootParent->getScale()));

	Container->addUniform("uLightPosition", BindUniform(CProgramContext::get().Scene.LightPosition));
	Container->setShader(CApplication::get().getSceneManager().getDefaultColorRenderPass(), CProgramContext::get().Shaders.Glyph);
	Container->setCullingEnabled(false);
	

	for (auto it = DataSet.getValues().begin(); it != DataSet.getValues().end(); ++ it)
	{
		CInstanceSceneObject::CInstance * Instance = Container->addInstance();

		float X = (float) ((it->getField(xField) - XRange.first) / (XRange.second - XRange.first));
		float Y = (float) ((it->getField(yField) - YRange.first) / (YRange.second - YRange.first));
		float Z = (float) ((it->getField(zField) - ZRange.first) / (ZRange.second - ZRange.first));

		Instance->setTranslation((SVector3f(X, Y, Z) - SVector3f(0.5f)) * DataScale * RootParent->getScale());
		Instance->setUniformOverride(CApplication::get().getSceneManager().getDefaultColorRenderPass(), "uColor", BindUniformValue(ColorMapper->getColor(* it)));
		//CRenderable::SMaterial mat;
		//mat.DiffuseColor = ColorMapper->getColor(* it);
		//Instance->setMaterial(mat);
	}
}
