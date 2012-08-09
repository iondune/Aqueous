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

public:

	struct SControl
	{
		int Mode;
		SVector3f SliceAxis;
		float LocalRange;
		float MinimumAlpha;
		float EmphasisLocation;
		float AlphaIntensity;

		SControl()
			: Mode(0), SliceAxis(1.f, 0.f, 0.f),
			LocalRange(0.1f), MinimumAlpha(0.1f),
			EmphasisLocation(0.5f), AlphaIntensity(1.f)
		{}
	};

	SControl Control;
	int ShowVolume;
	unsigned int VolumeHandle;


	CVolumeSceneObject();
	
	bool draw(IScene const * const scene, ERenderPass const Pass, bool const CullingEnabled);

};

#endif
