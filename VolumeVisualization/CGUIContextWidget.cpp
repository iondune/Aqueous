#include "CGUIContextWidget.h"

#include "CProgramContext.h"


CGUIContextWidget::CGUIContextWidget()
	: GUIContext(0), CGUIWidget(CProgramContext::Get().GUIContext)
{
	GUIContext = CProgramContext::Get().GUIContext;
}
