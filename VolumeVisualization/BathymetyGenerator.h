
#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "ProgressPrinter.h"


class BathymetryGenerator
{

public:

	void Generate(std::string const & FileName)
	{
		CImage * Image = CImage::Load("TerrainHeightImageSquare.tga");
		u32 const Width = Image->GetWidth();
		u32 const Height = Image->GetHeight();

		struct SPoint
		{
			f32 Height;
			vec2f Gradient;
			f32 Accumulator;
			bool Set;
		};

		std::vector<SPoint> Points(Width * Height);
		
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			color4i const Color = Image->GetPixel(x, y);

			Points[x + y * Width].Set = Color.Red > 0;
			Points[x + y * Width].Height = Color.Red;
			Points[x + y * Width].Accumulator = 0;
		}

		auto GetPoint = [Width, Height, Points](s32 x, s32 y) -> SPoint &
		{
			x = Clamp(x, 0, (s32) Width - 1);
			y = Clamp(y, 0, (s32) Height - 1);
			return Points[x + y * Width];
		};

		for (s32 y = 0; y < Height; ++ y)
		for (s32 x = 0; x < Width; ++ x)
		{
			if (GetPoint(x, y).Set && GetPoint(x, y - 1).Set && GetPoint(x + 1, y).Set && GetPoint(x, y + 1).Set && GetPoint(x - 1, y).Set)
				GetPoint(x, y).Gradient = vec2f(GetPoint(x + 1, y).Height - GetPoint(x - 1, y).Height, GetPoint(x, y + 1).Height - GetPoint(x, y - 1).Height) / 2.f;
			
		}
	}

};
