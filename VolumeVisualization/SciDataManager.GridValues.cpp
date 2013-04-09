#include "SciDataManager.h"

#include "RBFInterpolator/RBFInterpolator.h"


void SciDataManager::createGridDataFromRawValuesRBFI(Range AcceptedValues, double Deviations, std::string const & Field)
{
	int const Size = 20;
	GridDimensions[0] = Size;
	GridDimensions[1] = Size;
	GridDimensions[2] = Size;

	GridValues.clear();

	std::vector<float> X, Y, Z, F;

	Range XRange = RawValues.getValueRange("x", Deviations, AcceptedValues);
	Range YRange = RawValues.getValueRange("y", Deviations, AcceptedValues);
	Range ZRange = RawValues.getValueRange("z", Deviations, AcceptedValues);
	Range FRange = RawValues.getValueRange(Field, Deviations, AcceptedValues);

	for (auto it = RawValues.getValues().begin(); it != RawValues.getValues().end(); ++ it)
	{
		float x = (float) ((it->getField("x") - XRange.first) / (XRange.second - XRange.first));
		float y = (float) ((it->getField("y") - YRange.first) / (YRange.second - YRange.first));
		float z = (float) ((it->getField("z") - ZRange.first) / (ZRange.second - ZRange.first));
		float f = (float) ((it->getField(Field) - FRange.first) / (FRange.second - FRange.first));

		//if (! inRange(f, FRan

		if (f != f ||
			x != x ||
			y != y ||
			z != z ||
			! inRange(f, Range(0, 1)) ||
			! inRange(x, Range(0, 1)) ||
			! inRange(y, Range(0, 1)) ||
			! inRange(z, Range(0, 1)))
			continue;

		bool alreadyIn = false;
		for (u32 i = 0; i < X.size(); ++ i)
		{
			//if (equals(x, X[i]) || equals(y, Y[i]) || equals(z, Z[i]) || equals(f, F[i]))
			if (equals(x, X[i]) && equals(y, Y[i]) && equals(z, Z[i]) && equals(f, F[i]))
				alreadyIn = true;
		}

		if (! alreadyIn)
		{
			X.push_back(x);
			Y.push_back(y);
			Z.push_back(z);
			F.push_back(f);
		}
	}

	std::cout << "Interpolating " << X.size() << " values into volume." << std::endl;

	RBFInterpolator rbfi(X, Y, Z, F);
	
	for (int j = 0; j < Size; ++ j)//Size - 1; j >= 0; -- j)
	for (int k = 0; k < Size; ++ k)
	for (int i = 0; i < Size; ++ i)
	{
		SciData d(GridValues);
		d.addField(Field) = rbfi.interpolate(i / (float) Size, j / (float) Size, k / (float) Size);
		d.addField("x") = i / (float) Size;
		d.addField("y") = j / (float) Size;
		d.addField("z") = k / (float) Size;
	}
}

void SciDataManager::createGridDataFromRawValues(Range AcceptedValues, double Deviations, std::string const & Field)
{
	int const Size = 32;
	GridDimensions[0] = Size;
	GridDimensions[1] = Size*4;
	GridDimensions[2] = Size;

	GridValues.clear();

	static int const AccumulationRange = 5;
	
	Range XRange = RawValues.getValueRange("x", Deviations, AcceptedValues);
	Range YRange = RawValues.getValueRange("y", Deviations, AcceptedValues);
	Range ZRange = RawValues.getValueRange("z", Deviations, AcceptedValues);

	std::vector<SciData> Sorted = RawValues.getValues();
	std::sort(Sorted.begin(), Sorted.end(), [](SciData const & d1, SciData const & d2) { return d1.getPosition().Y < d2.getPosition().Y; });
	
	for (int j = 0; j < GridDimensions[0]; ++ j)
	for (int k = 0; k < GridDimensions[1]; ++ k)
	for (int i = 0; i < GridDimensions[2]; ++ i)
	{
		f64 const X = i / (f64) GridDimensions[0];
		f64 const Y = j / (f64) GridDimensions[1];
		f64 const Z = k / (f64) GridDimensions[2];

		f64 FieldAccumulator = 0.f;
		f64 NormalizationAccumulator = 0.f;
		f64 Normalization = 0.f;

		for (u32 t = 0; t < Sorted.size(); ++ t)
		{
			f64 const y = (f64) ((Sorted[t].getPosition().Y - YRange.first) / (YRange.second - YRange.first));

			if (y > Y)
			{
				for (u32 u = 0; u < AccumulationRange && t >= u; ++ u)
				{
					f64 const x = (f64) ((Sorted[t-u].getPosition().X - XRange.first) / (XRange.second - XRange.first));
					f64 const y = (f64) ((Sorted[t-u].getPosition().Y - YRange.first) / (YRange.second - YRange.first));
					f64 const z = (f64) ((Sorted[t-u].getPosition().Z - ZRange.first) / (ZRange.second - ZRange.first));
					Normalization = 1 / (sqrt(sq(X-x) + sq(Y-y) + sq(Z-z)) + 0.1f);
					FieldAccumulator += Sorted[t-u].getField(Field) * Normalization;
					NormalizationAccumulator += Normalization;
				}
				for (u32 u = 1; u < AccumulationRange && t + u < Sorted.size(); ++ u)
				{
					f64 const x = (f64) ((Sorted[t+u].getPosition().X - XRange.first) / (XRange.second - XRange.first));
					f64 const y = (f64) ((Sorted[t+u].getPosition().Y - YRange.first) / (YRange.second - YRange.first));
					f64 const z = (f64) ((Sorted[t+u].getPosition().Z - ZRange.first) / (ZRange.second - ZRange.first));
					Normalization = 1 / (sqrt(sq(X-x) + sq(Y-y) + sq(Z-z)) + 0.1f);
					FieldAccumulator += Sorted[t+u].getField(Field) * Normalization;
					NormalizationAccumulator += Normalization;
				}
				break;
			}
		}
		
		SciData d(GridValues);
		d.addField(Field) = FieldAccumulator / NormalizationAccumulator;
		d.addField("x") = X;
		d.addField("y") = Y;
		d.addField("z") = Z;
	}
}
