#ifndef _CVOLUMESCENEOBJECT_H_INCLUDED_
#define _CVOLUMESCENEOBJECT_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>


class CVolumeSceneObject : public ISceneObject
{
	
	// Cube used for volume rendering
	CMesh * VolumeCube;

	// Framebuffer used for volume rendering
	CFrameBufferObject * VolumeTarget;
	CTexture * VolumeBuffer;

	CSceneManager & SceneManager;

	CShader * Shader;

public:

	struct SControl
	{
		// ToDo: Provide actual ranges and use for volume control widget
		int Mode;
		SVector3f SliceAxis;
		float LocalRange;
		float MinimumAlpha;
		float EmphasisLocation;
		float AlphaIntensity;

		float StepSize;

		SControl()
			: Mode(0), SliceAxis(1.f, 0.f, 0.f),
			LocalRange(0.1f), MinimumAlpha(0.1f),
			EmphasisLocation(0.5f), AlphaIntensity(1.f),
			StepSize(25.f)
		{}
	};

	SControl Control;
	int ShowVolume;
	unsigned int VolumeHandle;


	CVolumeSceneObject();
	
	bool draw(IScene const * const scene, smartPtr<IRenderPass> Pass, bool const CullingEnabled);

};

#endif
