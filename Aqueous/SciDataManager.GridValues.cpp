#include "SciDataManager.h"

#include "RBFInterpolator/RBFInterpolator.h"


void SciDataManager::createGridDataFromRawValuesRBFI(SRange<f64> AcceptedValues, double Deviations, std::string const & Field)
{
	u32 const Size = 20;
	GridDimensions[0] = Size;
	GridDimensions[1] = Size;
	GridDimensions[2] = Size;

	GridValues.Clear();

	std::vector<float> X, Y, Z, F;

	SRange<f64> XRange = RawValues.GetFieldRange(RawValues.Traits.PositionXField, Deviations, AcceptedValues);
	SRange<f64> YRange = RawValues.GetFieldRange(RawValues.Traits.PositionYField, Deviations, AcceptedValues);
	SRange<f64> ZRange = RawValues.GetFieldRange(RawValues.Traits.PositionZField, Deviations, AcceptedValues);
	SRange<f64> FRange = RawValues.GetFieldRange(Field, Deviations, AcceptedValues);

	for (auto it = RawValues.GetValues().begin(); it != RawValues.GetValues().end(); ++ it)
	{
		f32 x = (f32) XRange.Normalize(it->GetField(RawValues.Traits.PositionXField));
		f32 y = (f32) YRange.Normalize(it->GetField(RawValues.Traits.PositionYField));
		f32 z = (f32) ZRange.Normalize(it->GetField(RawValues.Traits.PositionZField));
		f32 f = (f32) FRange.Normalize(it->GetField(Field));

		//if (! inRange(f, FRan

		if (f != f ||
			x != x ||
			y != y ||
			z != z ||
			! SRange<f32>(0, 1).Contains(f) ||
			! SRange<f32>(0, 1).Contains(x) ||
			! SRange<f32>(0, 1).Contains(y) ||
			! SRange<f32>(0, 1).Contains(z))
			continue;

		bool alreadyIn = false;
		for (u32 i = 0; i < X.size(); ++ i)
		{
			if (Equals(x, X[i]) || Equals(y, Y[i]) || Equals(z, Z[i]) || Equals(f, F[i]))
			//if (Equals(x, X[i]) && Equals(y, Y[i]) && Equals(z, Z[i]) && Equals(f, F[i]))
			{
				alreadyIn = true;
				printf("Rejected a point\n");
			}
		}

		if (! alreadyIn)
		{
			X.push_back(x);
			Y.push_back(y);
			Z.push_back(z);
			F.push_back(f);
			//printf("Accepted a point\n");
		}
	}

	std::cout << "Interpolating " << X.size() << " values into volume." << std::endl;

	RBFInterpolator rbfi(X, Y, Z, F);
	GridValues.AddField(Field);
	GridValues.AddField("x");
	GridValues.AddField("y");
	GridValues.AddField("z");
	
	for (int k = 0; k < Size; ++ k)
	for (int j = 0; j < Size; ++ j)
	for (int i = 0; i < Size; ++ i)
	{
		STable::SRow & Row = GridValues.AddRow();
		Row.GetField(Field) = rbfi.interpolate(i / (f32) Size, j / (f32) Size, k / (f32) Size);
		Row.GetField("x") = i / (f32) Size;
		Row.GetField("y") = j / (f32) Size;
		Row.GetField("z") = k / (f32) Size;
	}
}

void SciDataManager::createGridDataFromRawValues(SRange<f64> AcceptedValues, double Deviations, std::string const & Field)
{
	int const Size = 32;
	GridDimensions[0] = Size;
	GridDimensions[1] = Size*4;
	GridDimensions[2] = Size;

	GridValues.Clear();
	GridValues.AddField(Field);
	GridValues.AddField("x");
	GridValues.AddField("y");
	GridValues.AddField("z");

	static int const AccumulationRange = 5;
	
	SRange<f64> XRange = RawValues.GetFieldRange("x", Deviations, AcceptedValues);
	SRange<f64> YRange = RawValues.GetFieldRange("y", Deviations, AcceptedValues);
	SRange<f64> ZRange = RawValues.GetFieldRange("z", Deviations, AcceptedValues);

	std::vector<STable::SRow> Sorted = RawValues.GetValues();
	std::sort(Sorted.begin(), Sorted.end(), [](STable::SRow const & d1, STable::SRow const & d2) { return d1.GetPosition().Y < d2.GetPosition().Y; });
	
	for (u32 j = 0; j < GridDimensions[0]; ++ j)
	for (u32 k = 0; k < GridDimensions[1]; ++ k)
	for (u32 i = 0; i < GridDimensions[2]; ++ i)
	{
		f64 const X = i / (f64) GridDimensions[0];
		f64 const Y = j / (f64) GridDimensions[1];
		f64 const Z = k / (f64) GridDimensions[2];

		f64 FieldAccumulator = 0.f;
		f64 NormalizationAccumulator = 0.f;
		f64 Normalization = 0.f;

		for (u32 t = 0; t < Sorted.size(); ++ t)
		{
			f64 const y = YRange.Normalize(Sorted[t].GetPosition().Y);

			if (y > Y)
			{
				for (u32 u = 0; u < AccumulationRange && t >= u; ++ u)
				{
					f64 const x = XRange.Normalize(Sorted[t-u].GetPosition().X);
					f64 const y = YRange.Normalize(Sorted[t-u].GetPosition().Y);
					f64 const z = ZRange.Normalize(Sorted[t-u].GetPosition().Z);
					Normalization = 1 / (sqrt(Sq(X-x) + Sq(Y-y) + Sq(Z-z)) + 0.1f);
					FieldAccumulator += Sorted[t-u].GetField(Field) * Normalization;
					NormalizationAccumulator += Normalization;
				}
				for (u32 u = 1; u < AccumulationRange && t + u < Sorted.size(); ++ u)
				{
					f64 const x = XRange.Normalize(Sorted[t+u].GetPosition().X);
					f64 const y = YRange.Normalize(Sorted[t+u].GetPosition().Y);
					f64 const z = ZRange.Normalize(Sorted[t+u].GetPosition().Z);
					Normalization = 1 / (sqrt(Sq(X-x) + Sq(Y-y) + Sq(Z-z)) + 0.1f);
					FieldAccumulator += Sorted[t+u].GetField(Field) * Normalization;
					NormalizationAccumulator += Normalization;
				}
				break;
			}
		}
		
		STable::SRow & Row = GridValues.AddRow();
		Row.GetField(Field) = FieldAccumulator / NormalizationAccumulator;
		Row.GetField("x") = X;
		Row.GetField("y") = Y;
		Row.GetField("z") = Z;
	}
}
