
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "ProgressPrinter.h"
#include <math.h>


struct SClassification
{
	SColorc Color;
	float Value;
};

class ContourConverter
{

public:

	void convert()
	{
		CImage * Image = CImageLoader::loadTGAImage("../terrain_input.tga");

		unsigned char * Data = Image->getImageData();
		unsigned char * DataCopy = new unsigned char[Image->getWidth() * Image->getHeight() * 3];
		SClassification ** TruthTable = new SClassification *[Image->getWidth() * Image->getHeight()], 
			** TruthTableCopy = new SClassification *[Image->getWidth() * Image->getHeight()];

		// Load Classification List
		std::vector<SClassification> Classifications;

		SClassification Class;
		Class.Value = 0.f;
		Class.Color = SColorc(62, 156, 139);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(92, 0, 130);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(97, 30, 209);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(78, 85, 255);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(0, 45, 190);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(50, 214, 255);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(10, 194, 84);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(147, 255, 61);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(240, 255, 0);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(255, 138, 0);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(255, 0, 0);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(254, 125, 125);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(233, 85, 255);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(255, 255, 255);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(144, 144, 144);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(80, 71, 58);
		Classifications.push_back(Class);
		Class.Value += 1.f / 16.f;
		Class.Color = SColorc(103, 66, 10);
		Classifications.push_back(Class);

		SClassification * NullClass = new SClassification();
		NullClass->Value = -1.f;
		NullClass->Color = SColorc(0, 0, 0);

		printf("Pass 1 -> Posterization\n");

		// Use truth table to check for certain values
		for (int x = 0; x < Image->getWidth(); ++ x)
		{
			for (int y = 0; y < Image->getHeight(); ++ y)
			{
				TruthTable[x + y * Image->getWidth()] = 0;
			}
		}
		
		// Posterization Pass 1 -> assign certain or near certain class values
		for (int x = 0; x < Image->getWidth(); ++ x)
		{
			for (int y = 0; y < Image->getHeight(); ++ y)
			{
				SColorc Color = SColorc(
					Data[x * 3 + y * Image->getWidth() * 3 + 0], 
					Data[x * 3 + y * Image->getWidth() * 3 + 1], 
					Data[x * 3 + y * Image->getWidth() * 3 + 2]);

				if (Color == NullClass->Color)
				{
					TruthTable[x + y * Image->getWidth()] = NullClass;
				}
				else
				{
					for (auto it = Classifications.begin(); it != Classifications.end(); ++ it)
					{
						if (Color == it->Color)
						{
							TruthTable[x + y * Image->getWidth()] = & * it;
						}
					}
				}

				if (! TruthTable[x + y * Image->getWidth()])
				{
					auto getColorDistance = [](SColorc const & c1, SColorc const & c2) -> float
					{
						return (float) abs(c1.Red - c2.Red) + abs(c1.Green - c2.Green) + abs(c1.Blue - c2.Blue);
					};

					float MinDistance = getColorDistance(Color, SColorc(0, 0, 0)) * 0.5f;
					SClassification * MinClass = 0;

					for (auto it = Classifications.begin(); it != Classifications.end(); ++ it)
					{
						float CheckDistance = getColorDistance(Color, it->Color);
						if (CheckDistance < MinDistance)
						{
							MinDistance = CheckDistance;
							MinClass = & * it;
						}
					}

					if (MinDistance < 1.f)
					{
						//printf("Near certain value!\n");
						if (MinClass)
							;//TruthTable[x + y * Image->getWidth()] = MinClass;
						else
							TruthTable[x + y * Image->getWidth()] = NullClass;
					}
				}
			}
		}

		// Posterization Pass 2 -> creep certain areas
		int MaximumCreepCounters = 200;
		while (-- MaximumCreepCounters != 0)
		{
			for (int x = 0; x < Image->getWidth(); ++ x)
			{
				for (int y = 0; y < Image->getHeight(); ++ y)
				{
					SColorc Color = SColorc(0, 0, 0);

					if (! TruthTable[x + y * Image->getWidth()])
					{
						for (int u = -1; u <= 1; ++ u)
						{
							for (int v = -1; v <= 1; ++ v)
							{
								bool const Distance = abs(u) + abs(v) == 1;
								bool const InBounds = (x + u) >= 0 && (x + u) < Image->getWidth() && (y + v) >= 0 && (y + v) < Image->getHeight();
								if (Distance && InBounds)
								{
									if (TruthTable[(x + u) + (y + v) * Image->getWidth()] && TruthTable[(x + u) + (y + v) * Image->getWidth()] != NullClass)
									{
										TruthTable[x + y * Image->getWidth()] = TruthTable[(x + u) + (y + v) * Image->getWidth()];
									}
								}
							}
						}
					}
				}
			}
		}

		// Posterization Pass 3 -> colorize output
		for (int x = 0; x < Image->getWidth(); ++ x)
		{
			for (int y = 0; y < Image->getHeight(); ++ y)
			{
				SColorc Color = SColorc(0, 0, 0);

				if (TruthTable[x + y * Image->getWidth()])
					Color = TruthTable[x + y * Image->getWidth()]->Color;

				Data[x * 3 + y * Image->getWidth() * 3 + 0] = Color.Red * 255;
				Data[x * 3 + y * Image->getWidth() * 3 + 1] = Color.Green * 255;
				Data[x * 3 + y * Image->getWidth() * 3 + 2] = Color.Blue * 255;
			}
		}

		Image->write("terrain_output_1_posterized.bmp");

		printf("Pass 2 -> Edge Removal\n");
		
		memcpy(TruthTableCopy, TruthTable, Image->getWidth() * Image->getHeight() * sizeof(SClassification *));
		// Edge Removal Pass 1 -> Detect unset or null set areas and find closest value to choose
		for (int x = 0; x < Image->getWidth(); ++ x)
		{
			for (int y = 0; y < Image->getHeight(); ++ y)
			{
				if (TruthTable[x + y * Image->getWidth()] == NullClass)
					TruthTable[x + y * Image->getWidth()] = 0;

				if (! TruthTable[x + y * Image->getWidth()])
				{
					for (int i = 0; i < 50; ++ i)
					{
						for (int u = -i - 1; u <= i + 1; ++ u)
						{
							for (int v = -i - 1; v <= i + 1; ++ v)
							{
								bool const Distance = (u*u) + (v*v) == (i*i);
								bool const InBounds = (x + u) >= 0 && (x + u) < Image->getWidth() && (y + v) >= 0 && (y + v) < Image->getHeight();
								if (Distance && InBounds)
								{
									if (TruthTableCopy[(x + u) + (y + v) * Image->getWidth()] && TruthTableCopy[(x + u) + (y + v) * Image->getWidth()] != NullClass)
									{
										TruthTable[x + y * Image->getWidth()] = TruthTableCopy[(x + u) + (y + v) * Image->getWidth()];
									}
								}

								if (TruthTable[x + y * Image->getWidth()])
									break;
							}

							if (TruthTable[x + y * Image->getWidth()])
								break;
						}

						if (TruthTable[x + y * Image->getWidth()])
							break;
					}
				}
			}
		}

		// Edge Removal Pass 2 -> colorize output
		for (int x = 0; x < Image->getWidth(); ++ x)
		{
			for (int y = 0; y < Image->getHeight(); ++ y)
			{
				SColorc Color = SColorc(0, 0, 0);

				if (TruthTable[x + y * Image->getWidth()])
					Color = TruthTable[x + y * Image->getWidth()]->Color;
				else
					printf("Error! Unepexted non-classified point in output!\n");

				Data[x * 3 + y * Image->getWidth() * 3 + 0] = Color.Red * 255;
				Data[x * 3 + y * Image->getWidth() * 3 + 1] = Color.Green * 255;
				Data[x * 3 + y * Image->getWidth() * 3 + 2] = Color.Blue * 255;
			}
		}

		Image->write("terrain_output_2_stitched.bmp");

		printf("Pass 3 -> Value Interpolation\n");
		ProgressPrinter p;
		p.begin();
		for (int x = 0; x < Image->getWidth(); ++ x)
		{
			p.update(x * 100 / Image->getWidth());
			for (int y = 0; y < Image->getHeight(); ++ y)
			{
				if (! TruthTable[x + y * Image->getWidth()])
					TruthTable[x + y * Image->getWidth()] = NullClass;

				SClassification * Class = TruthTable[x + y * Image->getWidth()], * HigherClass = 0, * LowerClass = 0, * TestClass;
				SVector2i HigherPos, LowerPos;

				if (! equals(Class->Value, 0.f))
				{
					for (int i = 0; i < 120;)
					{
						for (int u = -i - 1; u <= i + 1; ++ u)
						{
							for (int v = -i - 1; v <= i + 1; ++ v)
							{
								if ((u*u) + (v*v) != (i*i))
									continue;

								if (! ((x + u) >= 0 && (x + u) < Image->getWidth() && (y + v) >= 0 && (y + v) < Image->getHeight()))
									continue;

								TestClass = TruthTable[(x + u) + (y + v) * Image->getWidth()];

								if (TestClass == Class)
									continue;
								
								if (! HigherClass && TestClass->Value > Class->Value)
								{
									HigherClass = TestClass;
									HigherPos = SVector2i(u, v);
								}

								if (! LowerClass && TestClass->Value < Class->Value)
								{
									LowerClass = TestClass;
									LowerPos = SVector2i(u, v);
								}

								if (HigherClass && LowerClass)
									break;
							}

							if (HigherClass && LowerClass)
								break;
						}

						if (HigherClass && LowerClass)
							break;
						if (i < 20)
							i += 1;
						else if (i < 40)
							i += 2;
						else if (i < 60)
							i += 3;
						else if (i < 80)
							i += 4;
						else if (i < 100)
							i += 5;
						else //if (i < 120)
							i += 6;
					}
				}
				
				float Value = Class->Value;

				//if ((! HigherClass || ! LowerClass) && ! equals(Class->Value, 0.f))
					//printf("Unabled to find expected class region!\n");

				if (HigherClass && LowerClass)
				{
					float LowDistance = sqrt((float)((LowerPos.X*LowerPos.X) + (LowerPos.Y*LowerPos.Y)));
					float HighDistance = sqrt((float)((HigherPos.X*HigherPos.X) + (HigherPos.Y*HigherPos.Y)));

					float Ratio = LowDistance / (HighDistance + LowDistance);
					Value = Value * Ratio + LowerClass->Value * (1.f - Ratio);

					DataCopy[x * 3 + y * Image->getWidth() * 3 + 0] = 
					DataCopy[x * 3 + y * Image->getWidth() * 3 + 1] = 
					DataCopy[x * 3 + y * Image->getWidth() * 3 + 2] = 
					Data[x * 3 + y * Image->getWidth() * 3 + 0] = 
					Data[x * 3 + y * Image->getWidth() * 3 + 1] = 
					Data[x * 3 + y * Image->getWidth() * 3 + 2] = (unsigned char) (Value * 255);
				}
				else if (equals(Class->Value, 0.f))
				{
					DataCopy[x * 3 + y * Image->getWidth() * 3 + 0] = 
					DataCopy[x * 3 + y * Image->getWidth() * 3 + 1] = 
					DataCopy[x * 3 + y * Image->getWidth() * 3 + 2] = 0;
					Data[x * 3 + y * Image->getWidth() * 3 + 0] = 0;
					Data[x * 3 + y * Image->getWidth() * 3 + 1] = 60;
					Data[x * 3 + y * Image->getWidth() * 3 + 2] = 160;
				}
				else
				{
					if (HigherClass || LowerClass)
					{
						float const DefaultDistance = 50.f;
						float LowDistance = LowerClass ? sqrt((float)((LowerPos.X*LowerPos.X) + (LowerPos.Y*LowerPos.Y))) : DefaultDistance;
						float HighDistance = HigherClass ? sqrt((float)((HigherPos.X*HigherPos.X) + (HigherPos.Y*HigherPos.Y))) : DefaultDistance;

						float Ratio = LowDistance / (HighDistance + LowDistance);
						if (LowerClass)
						{
							Value = Value * Ratio + LowerClass->Value * (1.f - Ratio);
							Data[x * 3 + y * Image->getWidth() * 3 + 0] = 0;
							Data[x * 3 + y * Image->getWidth() * 3 + 1] = 255;
							Data[x * 3 + y * Image->getWidth() * 3 + 2] = 0;
						}
						else
						{
							Value = Value * Ratio + (2 * Value - HigherClass->Value) * (1.f - Ratio);
							Data[x * 3 + y * Image->getWidth() * 3 + 0] = 255;
							Data[x * 3 + y * Image->getWidth() * 3 + 1] = 255;
							Data[x * 3 + y * Image->getWidth() * 3 + 2] = 0;
						}
					}
					else
					{
						Data[x * 3 + y * Image->getWidth() * 3 + 0] = 255;
						Data[x * 3 + y * Image->getWidth() * 3 + 1] = 0;
						Data[x * 3 + y * Image->getWidth() * 3 + 2] = 0;
					}
					
					DataCopy[x * 3 + y * Image->getWidth() * 3 + 0] = 
					DataCopy[x * 3 + y * Image->getWidth() * 3 + 1] = 
					DataCopy[x * 3 + y * Image->getWidth() * 3 + 2] = (unsigned char) (Value * 255);
				}
				
				
			}
		}
		p.end();
		Image->write("terrain_output_3_interpolated.bmp");

		for (int x = 0; x < Image->getWidth(); ++ x)
		{
			for (int y = 0; y < Image->getHeight(); ++ y)
			{
				Data[x * 3 + y * Image->getWidth() * 3 + 0] = DataCopy[x * 3 + y * Image->getWidth() * 3 + 0];
				Data[x * 3 + y * Image->getWidth() * 3 + 1] = DataCopy[x * 3 + y * Image->getWidth() * 3 + 1];
				Data[x * 3 + y * Image->getWidth() * 3 + 2] = DataCopy[x * 3 + y * Image->getWidth() * 3 + 2];
			}
		}
		Image->write("terrain_output_4_final.bmp");
	}

};
