#ifndef _CABBAGE_SCENE_SATTRIBUTE_H_INCLUDED_
#define _CABBAGE_SCENE_SATTRIBUTE_H_INCLUDED_

#include "CShaderContext.h"
#include "CBufferObject.h"

struct IAttribute
{
	virtual void bind(GLint const handle, CShaderContext & shaderContext) const =0;
	virtual void load() const =0;
};

template <typename T>
struct SAttribute : public IAttribute
{
	CBufferObject<T> * Buffer;
	int ElementSize;

	SAttribute()
		: Buffer(0), ElementSize(0)
	{}

	SAttribute(CBufferObject<T> * buffer, int const elementSize = 4)
		: Buffer(buffer), ElementSize(elementSize)
	{}

	void load() const
	{
		if (Buffer && Buffer->isDirty())
			Buffer->syncData();
	}

	void bind(GLint const handle, CShaderContext & shaderContext) const
	{
		if (Buffer)
		{
			if (Buffer->isDirty())
				Buffer->syncData();

			shaderContext.bindBufferObject(handle, Buffer->getHandle(), ElementSize);
		}
	}
};

#endif
