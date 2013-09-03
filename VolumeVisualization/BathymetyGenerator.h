
#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "ProgressPrinter.h"


class BathymetryGenerator
{

public:

	void Generate()
	{
		CImage * Image = CImage::Load("TerrainHeightImageSquare.bmp");
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

		auto GetPoint = [&Width, &Height, &Points](s32 x, s32 y) -> SPoint &
		{
			x = Clamp(x, 0, (s32) Width - 1);
			y = Clamp(y, 0, (s32) Height - 1);
			return Points[x + y * Width];
		};

		for (s32 y = 0; y < (s32) Height; ++ y)
		for (s32 x = 0; x < (s32) Width; ++ x)
		{
			if (GetPoint(x, y).Set && GetPoint(x, y - 1).Set && GetPoint(x + 1, y).Set && GetPoint(x, y + 1).Set && GetPoint(x - 1, y).Set)
			{
				GetPoint(x, y).Gradient = vec2f(GetPoint(x + 1, y).Height - GetPoint(x - 1, y).Height, GetPoint(x, y + 1).Height - GetPoint(x, y - 1).Height) / 2.f;
			}
			else
			{
				GetPoint(x, y).Set = false;
			}
		}

		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			vec2f GradNormal = GetPoint(x, y).Gradient.GetNormalized();
			Image->SetPixel(x, y, color4i(Clamp<s32>(GradNormal.X * 128 + 128, 0, 255), Clamp<s32>(GradNormal.Y * 128 + 128, 0, 255), 0, 0));
		}
		Image->Write("OutputPreNormals.bmp");

		printf("Calculating gradients...");
		ProgressPrinter P;
		P.Begin();
		for (s32 y = 0; y < (s32) Height; ++ y)
		for (s32 x = 0; x < (s32) Width; ++ x)
		{
			if (! GetPoint(x, y).Set)
			{
				f32 Accumulator = 0;
				s32 const SquareSize = 10;
				for (s32 i = - SquareSize / 2; i < SquareSize / 2; ++ i)
				for (s32 j = - SquareSize / 2; j < SquareSize / 2; ++ j)
				{
					if (GetPoint(x+i, y+j).Set)
					{
						vec2f const Offset(vec2i(i, j));

						f32 const Weight = (1 / Offset.Length()) * Offset.Dot(GetPoint(x+i, y+j).Gradient);
						GetPoint(x, y).Gradient += Weight * GetPoint(x+i, y+j).Gradient;
						Accumulator += Weight;
					}
				}

				GetPoint(x, y).Gradient /= Accumulator;
			}

			P.Update(100 * y / Height);
		}
		P.End();

		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			vec2f GradNormal = GetPoint(x, y).Gradient.GetNormalized();
			Image->SetPixel(x, y, color4i(Clamp<s32>(GradNormal.X * 128 + 128, 0, 255), Clamp<s32>(GradNormal.Y * 128 + 128, 0, 255), 0, 0));
		}
		Image->Write("OutputNormals.bmp");

		printf("Calculating Heights...");
		P.Begin();
		for (s32 y = 0; y < (s32) Height; ++ y)
		for (s32 x = 0; x < (s32) Width; ++ x)
		{
			if (! GetPoint(x, y).Set)
			{
				GetPoint(x, y).Height = 0;

				f32 Accumulator = 0;
				s32 const SquareSize = 4;
				for (s32 i = - SquareSize / 2; i < SquareSize / 2; ++ i)
				for (s32 j = - SquareSize / 2; j < SquareSize / 2; ++ j)
				{
					if (GetPoint(x+i, y+j).Set)
					{
						vec2f const Offset(vec2i(i, j));
						f32 const Weight = (1 / Offset.Length()) * Offset.Dot(GetPoint(x+i, y+j).Gradient);

						GetPoint(x, y).Height += Weight * (GetPoint(x+i, y+j).Height + Offset.Length() * Offset.Dot(GetPoint(x+i, y+j).Gradient));
						Accumulator += Weight;
					}
				}

				if (Accumulator > 0)
				{
					GetPoint(x, y).Height /= Accumulator;
					GetPoint(x, y).Set = true;
				}
			}

			P.Update(100 * y / Height);
		}
		P.End();

		
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			u8 ColorValue = Clamp<s32>(GetPoint(x, y).Height, 0, 255);
			Image->SetPixel(x, y, color4i(ColorValue));
		}

		Image->Write("Output.bmp");
	}
};
