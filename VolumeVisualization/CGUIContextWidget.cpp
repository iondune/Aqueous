#include "CGUIContextWidget.h"

#include "CProgramContext.h"


CGUIContextWidget::CGUIContextWidget()
	: CGUIWidget(CProgramContext::get().GUIContext)
{
}
