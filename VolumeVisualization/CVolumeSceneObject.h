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
		// To Do : Provide actual ranges and use for volume control widget
		int Mode;
		bool Debug;
		SVector3f SliceAxis;
		float LocalRange;
		float MinimumAlpha;
		float EmphasisLocation;
		float AlphaIntensity;

		float StepSize;

		SControl();
	};

	SControl Control;
	int ShowVolume;
	unsigned int VolumeHandle;


	CVolumeSceneObject();
	
	bool draw(IScene const * const scene, sharedPtr<IRenderPass> Pass, bool const CullingEnabled);

};

#endif
