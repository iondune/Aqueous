
#include "CGUIContextWidget.h"
#include "CProgramContext.h"


CGUIContextWidget::CGUIContextWidget()
	: GUIContext(), CGUIWidget(CProgramContext::Get().GUIContext->Manager.Get())
{
	GUIContext = CProgramContext::Get().GUIContext;
}
