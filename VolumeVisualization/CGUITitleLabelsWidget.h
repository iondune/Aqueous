
#pragma once

#include "CGUIContextWidget.h"


class SciDataManager;

class CGUITitleLabelsWidget : public CGUIContextWidget
{

private:

	Gwen::Controls::Label * VolumeRangeIndicator;
	Gwen::Controls::Label * VolumeCalculationIndicator;

public:

	CGUITitleLabelsWidget(SciDataManager * DataManager);

	void resetVolumeRangeIndicator(SciDataManager * DataManager);
	void clearVolumeRangeIndicator();

};
