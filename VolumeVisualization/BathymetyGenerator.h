
#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>
#include <ionScience.h>

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
			f32 Height, PingPongHeight;
			vec2f Gradient, PingPongGradient;
			f32 Accumulator;
			bool Set, GradientSet, NewGradientSet, NewHeightSet;
		};

		std::vector<SPoint> Points(Width * Height);
		auto GetPoint = [&Width, &Height, &Points](s32 x, s32 y) -> SPoint &
		{
			x = Clamp(x, 0, (s32) Width - 1);
			y = Clamp(y, 0, (s32) Height - 1);
			return Points[x + y * Width];
		};
		
		// Load Pass
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			color4i const Color = Image->GetPixel(x, y);
			
			GetPoint(x, y).Set = (Color.Red > 0);
			GetPoint(x, y).Height = Color.Red;
			GetPoint(x, y).Accumulator = 0;
			GetPoint(x, y).GradientSet = false;
			GetPoint(x, y).NewGradientSet = false;
		}

		// Diagnostic
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			Image->SetPixel(x, y, GetPoint(x, y).Set ? Colors::Red : Colors::Black);
		}
		Image->Write("OutputSetMap.bmp");
		/*
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			u8 ColorValue = Clamp<s32>(GetPoint(x, y).Height, 0, 255);
			Image->SetPixel(x, y, color4i(ColorValue));
		}
		Image->Write("OutputOriginal.bmp");
		*/
		
		// Blur Pass
		printf("Guassian blur... ");
		ProgressPrinter P(50);
		P.Begin();
		static s32 const Sigma = 6;
		static s32 const Spread = 3;
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			GetPoint(x, y).PingPongHeight = 0;
			for (s32 i = 1; i < Sigma * Spread; ++ i)
				GetPoint(x, y).PingPongHeight += Gaussian<f32>(i, Sigma) * (GetPoint(x+i, y).Height + GetPoint(x-i, y).Height);
			GetPoint(x, y).PingPongHeight += Gaussian<f32>(0, Sigma) * GetPoint(x, y).Height;
			P.Update(50 * y / Height);
		}
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			GetPoint(x, y).Height = GetPoint(x, y).PingPongHeight;
		}
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			GetPoint(x, y).PingPongHeight = 0;
			for (s32 i = 1; i < Sigma * Spread; ++ i)
				GetPoint(x, y).PingPongHeight += Gaussian<f32>(i, Sigma) * (GetPoint(x, y+i).Height + GetPoint(x, y-i).Height);
			GetPoint(x, y).PingPongHeight += Gaussian<f32>(0, Sigma) * GetPoint(x, y).Height;
			P.Update(50 + 50 * y / Height);
		}
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			GetPoint(x, y).Height = GetPoint(x, y).PingPongHeight;
		}
		P.End();
		
		// Diagnostic
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			u8 ColorValue = Clamp<s32>(GetPoint(x, y).Height, 0, 255);
			Image->SetPixel(x, y, color4i(ColorValue));
		}
		Image->Write("OutputBlurred.bmp");

		// Gradient Pass
		printf("Calculating gradients... ");
		P.Begin();
		s32 SetCount = 0;
		for (s32 y = 0; y < (s32) Height; ++ y)
		for (s32 x = 0; x < (s32) Width; ++ x)
		{
			if (GetPoint(x, y).Set && GetPoint(x, y - 1).Set && GetPoint(x + 1, y).Set && GetPoint(x, y + 1).Set && GetPoint(x - 1, y).Set)
			{
				GetPoint(x, y).Gradient = vec2f(GetPoint(x + 1, y).Height - GetPoint(x - 1, y).Height, GetPoint(x, y + 1).Height - GetPoint(x, y - 1).Height) / 2.f;
				GetPoint(x, y).GradientSet = true;
				SetCount ++;
			}

			P.Update(100 * y / Height);
		}
		P.End();
		
		// Diagnostic
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			vec2f GradNormal = GetPoint(x, y).Gradient.GetNormalized();
			Image->SetPixel(x, y, color4i(Clamp<s32>(GradNormal.X * 128 + 128, 0, 255), Clamp<s32>(GradNormal.Y * 128 + 128, 0, 255), 0, 0));
		}
		Image->Write("OutputPreNormals.bmp");
		f32 MaxMagnitude = 0;
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
			MaxMagnitude = Maximum(MaxMagnitude, GetPoint(x, y).Gradient.Length());
		//printf("Maximum pre normal mag is %f\n", MaxMagnitude);
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			Image->SetPixel(x, y, color4f(GetPoint(x, y).Gradient.Length() / MaxMagnitude));
		}
		Image->Write("OutputPreNormalsMagnitude.bmp");

		//FILE * f = fopen("Output.txt", "w");

		// Gradient bleed
		printf("Bleeding gradients... ");
		P.Begin();
		s32 CalculatedCount = 0;
		static s32 const SquareSize = 5;
		static s32 const Passes = 500;
		for (s32 t = 0; t < Passes && CalculatedCount < SetCount; ++ t)
		{
			for (s32 y = 0; y < (s32) Height; ++ y)
			for (s32 x = 0; x < (s32) Width; ++ x)
			{
				if (! GetPoint(x, y).GradientSet)
				{
					GetPoint(x, y).Gradient = vec2f();

					f32 Accumulator = 0;
					u32 Contributors = 0;
					for (s32 i = - SquareSize / 2; i < SquareSize / 2; ++ i)
					for (s32 j = - SquareSize / 2; j < SquareSize / 2; ++ j)
					{
						if (i != 0 || j != 0)
						{
							vec2f const Offset(vec2i(i, j));
							if (Offset.LengthSq() <= Sq(SquareSize) && GetPoint(x+i, y+j).GradientSet)
							{
								f32 const Weight = (1 / Offset.Length()) * (Offset.GetNormalized().Dot(GetPoint(x+i, y+j).Gradient.GetNormalized()) + 0.2f);
								GetPoint(x, y).Gradient += Weight * GetPoint(x+i, y+j).Gradient;
								Accumulator += Weight;
								Contributors ++;
							}
						}
					}

					if (Contributors >= 3 && Accumulator >= 0.1f)
					{
						f32 OriginalLength = GetPoint(x, y).Gradient.Length();
						GetPoint(x, y).Gradient /= Accumulator;
						//fprintf(f, "%04d %f : [%d] (%d, %f) %f\n", 
						//	(s32) GetPoint(x, y).Gradient.Length(), GetPoint(x, y).Gradient.Length(), t, Contributors, Accumulator, OriginalLength);
						GetPoint(x, y).NewGradientSet = true;
						CalculatedCount ++;
					}
				}

				P.Update(100 * CalculatedCount / SetCount);
			}

			for (s32 y = 0; y < (s32) Height; ++ y)
			for (s32 x = 0; x < (s32) Width; ++ x)
			{
				if (GetPoint(x, y).NewGradientSet)
					GetPoint(x, y).GradientSet = true;
			}
		}
		P.End();
		//fclose(f);
		
		// Diagnostic
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			vec2f GradNormal = GetPoint(x, y).Gradient.GetNormalized();
			Image->SetPixel(x, y, color4i(Clamp<s32>(GradNormal.X * 128 + 128, 0, 255), Clamp<s32>(GradNormal.Y * 128 + 128, 0, 255), 0, 0));
		}
		Image->Write("OutputPostNormals.bmp");
		MaxMagnitude = 0;
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
			MaxMagnitude = Maximum(MaxMagnitude, GetPoint(x, y).Gradient.Length());
		//printf("Maximum post normal mag is %f\n", MaxMagnitude);
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			Image->SetPixel(x, y, color4f(GetPoint(x, y).Gradient.Length() / MaxMagnitude));
		}
		Image->Write("OutputPostNormalsMagnitude.bmp");


		// Blur gradient Pass
		printf("Gradient blur... ");
		P.Begin();
		static s32 const GradientSigma = 3;//30;
		static s32 const GradientSpread = 3;
		static u32 const GradientBlurPasses = 3;
		for (u32 t = 0; t < GradientBlurPasses; ++ t)
		{
			for (u32 y = 0; y < Height; ++ y)
			for (u32 x = 0; x < Width; ++ x)
			{
				GetPoint(x, y).PingPongGradient = 0;
				for (s32 i = 1; i < GradientSigma * GradientSpread; ++ i)
					GetPoint(x, y).PingPongGradient += Gaussian<f32>(i, GradientSigma) * (GetPoint(x+i, y).Gradient + GetPoint(x-i, y).Gradient);
				GetPoint(x, y).PingPongGradient += Gaussian<f32>(0, GradientSigma) * GetPoint(x, y).Gradient;
				P.Update(100 * y / Height / 2 / GradientBlurPasses + 100 * t / GradientBlurPasses);
			}
			for (u32 y = 0; y < Height; ++ y)
			for (u32 x = 0; x < Width; ++ x)
			{
				GetPoint(x, y).Gradient = GetPoint(x, y).PingPongGradient;
			}
			for (u32 y = 0; y < Height; ++ y)
			for (u32 x = 0; x < Width; ++ x)
			{
				GetPoint(x, y).PingPongGradient = 0;
				for (s32 i = 1; i < GradientSigma * GradientSpread; ++ i)
					GetPoint(x, y).PingPongGradient += Gaussian<f32>(i, GradientSigma) * (GetPoint(x, y+i).Gradient + GetPoint(x, y-i).Gradient);
				GetPoint(x, y).PingPongGradient += Gaussian<f32>(0, GradientSigma) * GetPoint(x, y).Gradient;
				P.Update(100 * (y + Height) / Height / 2 / GradientBlurPasses + 100 * t / GradientBlurPasses);
			}
			for (u32 y = 0; y < Height; ++ y)
			for (u32 x = 0; x < Width; ++ x)
			{
				GetPoint(x, y).Gradient = GetPoint(x, y).PingPongGradient;
			}
		}
		P.End();
		
		// Diagnostic
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			vec2f GradNormal = GetPoint(x, y).Gradient.GetNormalized();
			Image->SetPixel(x, y, color4i(Clamp<s32>(GradNormal.X * 128 + 128, 0, 255), Clamp<s32>(GradNormal.Y * 128 + 128, 0, 255), 0, 0));
		}
		Image->Write("OutputNormalsGaussian.bmp");
		MaxMagnitude = 0;
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
			MaxMagnitude = Maximum(MaxMagnitude, GetPoint(x, y).Gradient.Length());
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			Image->SetPixel(x, y, color4f(GetPoint(x, y).Gradient.Length() / MaxMagnitude));
		}
		Image->Write("OutputNormalsGaussianMagnitude.bmp");
		

		f32 MinHeight = 0, MaxHeight = 0;
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			MinHeight = Minimum(MinHeight, GetPoint(x, y).Height);
			MaxHeight = Maximum(MaxHeight, GetPoint(x, y).Height);
		}
		printf("Minimum: %f Maximum: %f \n", MinHeight, MaxHeight);
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			Image->SetPixel(x, y, color4f((GetPoint(x, y).Height - MinHeight)/(MaxHeight - MinHeight)));
		}

		Image->Write("OutputPreHeight.bmp");

		printf("Calculating Heights... ");
		P.Begin();
		static u32 const HeightPasses = 100;
		for (u32 t = 0; t < HeightPasses; ++ t)
		{
			for (s32 y = 0; y < (s32) Height; ++ y)
			for (s32 x = 0; x < (s32) Width; ++ x)
			{
				if (! GetPoint(x, y).Set)
				{
					GetPoint(x, y).Height = 0;

					f32 Accumulator = 0;
					s32 const SquareSize = 5;
					for (s32 i = - SquareSize / 2; i < SquareSize / 2; ++ i)
					for (s32 j = - SquareSize / 2; j < SquareSize / 2; ++ j)
					{
						if (i != 0 || j != 0)
						{
							vec2f const Offset(vec2i(i, j));
							if (Offset.LengthSq() <= Sq(SquareSize) && GetPoint(x+i, y+j).Set)
							{
								f32 const Weight = (1 / Offset.Length()) * Offset.GetNormalized().Dot(GetPoint(x+i, y+j).Gradient.GetNormalized());

								GetPoint(x, y).Height += Weight * (GetPoint(x+i, y+j).Height + Offset.Length() * Offset.Dot(GetPoint(x+i, y+j).Gradient));
								Accumulator += Weight;
							}
						}
					}

					if (Accumulator > 0)
					{
						GetPoint(x, y).Height /= Accumulator;
						GetPoint(x, y).NewHeightSet = true;
					}
				}

				P.Update(100 * y / Height / HeightPasses + 100 * t / HeightPasses);
			}
			
			for (s32 y = 0; y < (s32) Height; ++ y)
			for (s32 x = 0; x < (s32) Width; ++ x)
			{
				if (GetPoint(x, y).NewHeightSet)
					GetPoint(x, y).Set = true;
			}
		}
		P.End();

		// Diagnostic
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			Image->SetPixel(x, y, GetPoint(x, y).Set ? Colors::Red : Colors::Black);
		}
		Image->Write("OutputPostSetMap.bmp");

		
		MinHeight = -256;
		MaxHeight = 256;
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			//MinHeight = Minimum(MinHeight, GetPoint(x, y).Height);
			//MaxHeight = Maximum(MaxHeight, GetPoint(x, y).Height);
		}
		printf("Minimum: %f Maximum: %f \n", MinHeight, MaxHeight);
		for (u32 y = 0; y < Height; ++ y)
		for (u32 x = 0; x < Width; ++ x)
		{
			Image->SetPixel(x, y, color4f((GetPoint(x, y).Height - MinHeight)/(MaxHeight - MinHeight)));
		}

		Image->Write("Output.bmp");
		WaitForUser();
	}
};
