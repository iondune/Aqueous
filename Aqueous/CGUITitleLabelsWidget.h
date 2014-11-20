
#pragma once

#include "CGUIContextWidget.h"


class CSite;

class CGUITitleLabelsWidget : public CGUIContextWidget
{

private:

	Gwen::Controls::Label * VolumeRangeIndicator;
	Gwen::Controls::Label * VolumeCalculationIndicator;

public:

	CGUITitleLabelsWidget(CSite * CurrentSite);

	void resetVolumeRangeIndicator(CSite * CurrentSite);
	void clearVolumeRangeIndicator();

};
