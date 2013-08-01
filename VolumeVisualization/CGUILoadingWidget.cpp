#include "CGUILoadingWidget.h"


CGUILoadingWidget::CGUILoadingWidget(std::string const & Label)
{
	Window = new Gwen::Controls::WindowControl(GUIManager->getCanvas());
	Window->SetClosable(false);
	Window->SetBounds(300, 300, 400, 150);
	Window->SetTitle("Loading...");
	Window->MakeModal(true);
	Window->SetMinimumSize(Gwen::Point(300, 150));

	Gwen::Controls::Label * LabelControl = new Gwen::Controls::Label(Window);
	LabelControl->SetFont(GUIManager->getRegularFont());
	LabelControl->SetText(Gwen::UnicodeString(Label.begin(), Label.end()));
	LabelControl->SetBounds(10, 10, 600, 300);
	LabelControl->SetTextColor(Gwen::Color(35, 35, 55, 215));

	ProgressBar = new Gwen::Controls::ProgressBar(Window);
	ProgressBar->SetBounds(20, 50, 340, 40);
	ProgressBar->SetValue(0.5f);
}

void CGUILoadingWidget::setProgress(f32 const Progress)
{
	scopedLock Lock(ProgressMutex);

	if (ProgressBar)
		ProgressBar->SetValue(Progress);
}

void CGUILoadingWidget::update(f32 const Elapsed)
{
	scopedLock Lock(ProgressMutex);

	if (ProgressBar->GetValue() >= 1.f)
	{
		Window->DelayedDelete();
		ProgressBar = 0;
		Window = 0;
	}
}
