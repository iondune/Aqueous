#ifndef _CGUITITLELABELSWIDGET_H_INCLUDED_
#define _CGUITITLELABELSWIDGET_H_INCLUDED_

#include "CGUIContextWidget.h"


class SciDataManager;

class CGUITitleLabelsWidget : public CGUIContextWidget
{

private:

	Gwen::Controls::Label * VolumeRangeIndicator;

public:

	CGUITitleLabelsWidget();

	void resetVolumeRangeIndicator(SciDataManager * DataManager);
	void clearVolumeRangeIndicator();

};

#endif
