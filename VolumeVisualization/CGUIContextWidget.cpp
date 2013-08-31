
#include "CGUIContextWidget.h"
#include "CProgramContext.h"


CGUIContextWidget::CGUIContextWidget()
	: GUIContext(), CGUIWidget(CProgramContext::Get().GUIContext)
{
	GUIContext = CProgramContext::Get().GUIContext;
}
