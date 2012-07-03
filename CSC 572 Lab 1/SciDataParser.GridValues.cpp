#include "SciDataParser.h"

#include "RFBInterpolator/RBFInterpolator.h"
#include "ProgressPrinter.h"


void SciDataParser::createGridDataFromRawValues()
{


	int const Size = 10;
	GridDimensions = new int[3];
	GridDimensions[0] = Size;
	GridDimensions[1] = Size;
	GridDimensions[2] = Size;

	GridValues.Values.clear();

	std::vector<float> X, Y, Z, F;

	Range	XRange = RawValues.getValueRange("x", 5.0);
	Range	YRange = RawValues.getValueRange("y", 5.0);
	Range	ZRange = RawValues.getValueRange("z", 5.0);
	Range	FRange = RawValues.getValueRange("o2", 5.0, Range(-999999.0, 999999.0));


	ProgressPrinter p;

	printf("Interpolating volume data...\n");
	p.begin();

	for (auto it = RawValues.Values.begin(); it != RawValues.Values.end(); ++ it)
	{
		float x = (float) ((it->getField("x") - XRange.first) / (XRange.second - XRange.first));
		float y = (float) ((it->getField("y") - YRange.first) / (YRange.second - YRange.first));
		float z = (float) ((it->getField("z") - ZRange.first) / (ZRange.second - ZRange.first));
		float f = (float) ((it->getField("o2") - ZRange.first) / (ZRange.second - ZRange.first));

		X.push_back(x);
		Y.push_back(y);
		Z.push_back(z);
		F.push_back(f);
	}

	RBFInterpolator rbfi(X, Y, Z, F);
	
	for (int j = Size - 1; j >= 0; -- j)
	for (int k = 0; k < Size; ++ k)
	for (int i = 0; i < Size; ++ i)
	{
		p.update(k * 100 / Size);
		SciData d(i / (float) Size, j / (float) Size, k / (float) Size);
		d.ScalarFields["o2"] = rbfi.interpolate(i / (float) Size, j / (float) Size, k / (float) Size);

		GridValues.Values.push_back(d);
	}
	p.end();
}
