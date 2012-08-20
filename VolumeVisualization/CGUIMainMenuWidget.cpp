#include "CGUIMainMenuWidget.h"

#include "CProgramContext.h"


CGUIMainMenuWidget::CGUIMainMenuWidget()
{
	// Top Label
	Gwen::Controls::Label * BigLabel = new Gwen::Controls::Label(GUIManager->getCanvas());
	BigLabel->SetFont(GUIManager->getLargeFont());
	BigLabel->SetText(L"Main Menu");
	BigLabel->SetBounds(10, 10, 1590, 300);
	BigLabel->SetTextColor(Gwen::Color(235, 255, 235, 215));
}
