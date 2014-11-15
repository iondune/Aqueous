
#include "CGUISiteWidget.h"
#include "CSite.h"


CGUISiteWidget::CGUISiteWidget(CSite * Site)
{
	this->Site = Site;

	auto Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetTitle(Site->GetName());
	Window->SetBounds(30, 60, 650 + 40 + 30, 550);
	Window->MakeModal();

	auto OKButton = new Gwen::Controls::Button(Window);
	OKButton->SetText("Open");
	OKButton->SetBounds(10+130, 60+400+16, 200, 30);
	OKButton->onPress.Add(this, & CGUIMainMenuWidget::OnSelectDataSet);
}
