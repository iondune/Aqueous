#include "CPlaneGridSceneObject.h"

CPlaneGridSceneObject::CPlaneGridSceneObject(s32 const Size)
{
	enableDebugData(EDebugData::Wireframe);
	
	CBufferObject<f32> * Vertices = new CBufferObject<f32>();
	CBufferObject<f32> * Colors = new CBufferObject<f32>();

	for (s32 y = 0; y < Size; ++ y)
	{
		for (s32 x = 0; x < Size; ++ x)
		{
			Vertices->push_back((f32) x);
			Vertices->push_back((f32) y);

			Vertices->push_back((f32) x+1);
			Vertices->push_back((f32) y);

			Vertices->push_back((f32) x+1);
			Vertices->push_back((f32) y+1);

			Vertices->push_back((f32) x);
			Vertices->push_back((f32) y+1);
		}
	}

	Vertices->syncData();
	Colors->syncData();
	
	CRenderable * Renderable = new CRenderable(this);
	Renderable->setDrawType(EDrawElementType::Quads);
	Renderable->setElementCount(4*Size*Size);

	addRenderable(Renderable);
	addAttribute("aPosition", BindAttribute(*Vertices, 2));
	
	setTranslation(vec3f(-Size/2.f, 0, -Size/2.f));
	setCullingEnabled(false);
}
