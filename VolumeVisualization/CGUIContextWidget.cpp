#include "CGUIContextWidget.h"

#include "CProgramContext.h"


CGUIContextWidget::CGUIContextWidget()
	: GUIContext(0), CGUIWidget(CProgramContext::get().GUIContext)
{
	GUIContext = CProgramContext::get().GUIContext;
}
