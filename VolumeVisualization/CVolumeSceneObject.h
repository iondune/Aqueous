#ifndef _CVOLUMESCENEOBJECT_H_INCLUDED_
#define _CVOLUMESCENEOBJECT_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>


class CVolumeSceneObject : public ISceneObject
{
	
	CSceneManager & SceneManager;

	CMesh * Cube;
	CShader * Shader;

public:

	struct SControl
	{
		// To Do : Provide actual ranges and use for volume control widget
		int Mode;
		int DebugLevel;
		SVector3f SliceAxis;
		float LocalRange;
		float MinimumAlpha;
		float EmphasisLocation;
		float AlphaIntensity;

		float StepSize;

		SControl();
	};
	SControl Control;

	unsigned int VolumeHandle;

	CVolumeSceneObject();
	bool draw(IScene const * const scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled);

};

#endif
