#include "SciDataManager.h"

#include "CProgramContext.h"


void SciDataManager::createPointCloudObjects(bool FromRaw, ISceneObject * RootParent, SVector3f const DataScale, IColorMapper * ColorMapper, 
		std::string const & xField, std::string const & yField, std::string const & zField)
{
	SciDataSet & DataSet = FromRaw ? RawValues : GridValues;

	ColorMapper->preProcessValues(DataSet);

	Range	XRange = DataSet.getValueRange(xField, 5.0);
	Range	YRange = DataSet.getValueRange(yField, 5.0);
	Range	ZRange = DataSet.getValueRange(zField, 5.0);

	for (auto it = DataSet.getValues().begin(); it != DataSet.getValues().end(); ++ it)
	{
		CMeshSceneObject * Object = new CMeshSceneObject();
		Object->setMesh(CProgramContext::get().Scene.Cube);
		Object->setParent(RootParent);
		Object->setScale(SVector3f(1.f) / 32.f);

		float X = (float) ((it->getField(xField) - XRange.first) / (XRange.second - XRange.first));
		float Y = (float) ((it->getField(yField) - YRange.first) / (YRange.second - YRange.first));
		float Z = (float) ((it->getField(zField) - ZRange.first) / (ZRange.second - ZRange.first));

		Object->setTranslation((SVector3f(X, Y, Z) - SVector3f(0.5f)) * DataScale);

		CRenderable::SMaterial mat;
		mat.DiffuseColor = ColorMapper->getColor(* it);
		Object->setMaterial(mat);

		Object->addUniform("uLightPosition", BindUniform(CProgramContext::get().Scene.LightPosition));
		Object->setShader(ERenderPass::Default, CProgramContext::get().Scene.Shader);
		Object->setCullingEnabled(false);
	}
}
