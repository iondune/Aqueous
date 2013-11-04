
#pragma once

#include <ionEngine.h>


struct ColorMapObject;

class GifWriter
{

protected:

	vec2i const Size;
	struct Frame
	{
		std::vector<u8> Data;
		int Delay;

		Frame(u32 const Size)
			: Data(Size)
		{}
	};

	std::vector<Frame> Frames;
	ColorMapObject * OutputPalette;

public:

	GifWriter(vec2i const & Dimensions);

	bool AddFrame(u8 const * const data,  float deltaTime);
	bool Save(std::string const & fileName);

};
